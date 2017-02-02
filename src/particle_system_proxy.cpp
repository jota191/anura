/*
	Copyright (C) 2013-2014 by Kristina Simpson <sweet.kristas@gmail.com>
	
	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	   1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.

	   2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	   3. This notice may not be removed or altered from any source
	   distribution.
*/

#include "filesystem.hpp"
#include "module.hpp"
#include "particle_system_proxy.hpp"
#include "profile_timer.hpp"
#include "variant_utils.hpp"

#include "formula_callable.hpp"
#include "ParticleSystem.hpp"
#include "ParticleSystemAffectors.hpp"
#include "ParticleSystemEmitters.hpp"
#include "ParticleSystemUI.hpp"
#include "SceneGraph.hpp"
#include "SceneNode.hpp"
#include "RenderManager.hpp"
#include "WindowManager.hpp"

namespace graphics
{
	using namespace KRE;

	ParticleSystemContainerProxy::ParticleSystemContainerProxy(const variant& node)
		: scene_(SceneGraph::create("ParticleSystemContainerProxy")),
		  root_(scene_->getRootNode()),
		  rmanager_(),
		  particle_system_container_(),
		  last_process_time_(-1),
		  running_(true),
		  enable_mouselook_(false),
		  invert_mouselook_(false)
	{
		root_->setNodeName("root_node");

		rmanager_ = std::make_shared<RenderManager>();
		rmanager_->addQueue(0, "PS");

		particle_system_container_ = Particles::ParticleSystemContainer::create(scene_, node);
		root_->attachNode(particle_system_container_);

		auto& psystem = particle_system_container_->getParticleSystem();
		if(psystem) {
			psystem->fastForward();
		}
	}

	void ParticleSystemContainerProxy::draw(const WindowPtr& wnd) const
	{
		if(running_) {
			scene_->renderScene(rmanager_);
			rmanager_->render(wnd);
#ifdef USE_IMGUI
			static std::vector<std::string> ifiles;
			// XX should update this if the directory contents changes.
			if(ifiles.empty()) {
				module::get_files_in_dir("images", &ifiles, nullptr);
			}
			KRE::Particles::ParticleUI(particle_system_container_, &enable_mouselook_, &invert_mouselook_, ifiles);
#endif

		}
	}

	void ParticleSystemContainerProxy::process()
	{
		if(!running_) {
			last_process_time_ = profile::get_tick_time();
			return;
		}
		ASSERT_LOG(scene_ != nullptr, "Scene is empty");
		float delta_time = 0.0f;
		if(last_process_time_ == -1) {
			last_process_time_ = profile::get_tick_time();
		}
		auto current_time = profile::get_tick_time();
		delta_time = (current_time - last_process_time_) / 1000.0f;
		scene_->process(delta_time);
		last_process_time_ = current_time;
	}

	void ParticleSystemContainerProxy::surrenderReferences(GarbageCollector* collector)
	{
		
	}

	class ParticleSystemProxy : public game_logic::FormulaCallable
	{
	public:
		explicit ParticleSystemProxy(KRE::Particles::ParticleSystemPtr obj) : obj_(obj)
		{}
	private:
		DECLARE_CALLABLE(ParticleSystemProxy);
		KRE::Particles::ParticleSystemPtr obj_;
	};

	class ParticleEmitterProxy : public game_logic::FormulaCallable
	{
	public:
		explicit ParticleEmitterProxy(KRE::Particles::EmitterPtr obj) : obj_(obj)
		{}
	private:
		DECLARE_CALLABLE(ParticleEmitterProxy);
		KRE::Particles::EmitterPtr obj_;
	};

	class ParticleAffectorProxy : public game_logic::FormulaCallable
	{
	public:
		explicit ParticleAffectorProxy(KRE::Particles::AffectorPtr obj) : obj_(obj)
		{}
	private:
		DECLARE_CALLABLE(ParticleAffectorProxy);
		KRE::Particles::AffectorPtr obj_;
	};

	BEGIN_DEFINE_CALLABLE_NOBASE(ParticleSystemContainerProxy)
		DEFINE_FIELD(running, "bool")
			return variant::from_bool(obj.running_);
		DEFINE_SET_FIELD
			obj.running_ = value.as_bool();
		DEFINE_FIELD(systems, "[builtin particle_system_proxy]")

			auto psystem = obj.particle_system_container_->getParticleSystem();
			return variant(new ParticleSystemProxy(psystem));

		DEFINE_FIELD(emitters, "[builtin particle_emitter_proxy]")

			auto psystem = obj.particle_system_container_->getParticleSystem();
			if(psystem) {
				auto emitter = psystem->getEmitter();
				return variant(new ParticleEmitterProxy(emitter));
			}
			return variant();

		DEFINE_FIELD(affectors, "[builtin particle_affector_proxy]")

			auto psystem = obj.particle_system_container_->getParticleSystem();
			if(psystem) {
				auto v = psystem->getAffectors();
				std::vector<variant> result;
				result.reserve(v.size());
				for(auto p : v) {
					result.emplace_back(variant(new ParticleAffectorProxy(p)));
				}
				return variant(&result);
			}
			return variant();

	END_DEFINE_CALLABLE(ParticleSystemContainerProxy)

	BEGIN_DEFINE_CALLABLE_NOBASE(ParticleSystemProxy)
	DEFINE_FIELD(addr, "string")
		char buf[64];
		sprintf(buf, "%p", obj.obj_.get());
		return variant(std::string(buf));
	END_DEFINE_CALLABLE(ParticleSystemProxy)

	BEGIN_DEFINE_CALLABLE_NOBASE(ParticleEmitterProxy)
	DEFINE_FIELD(addr, "string")
		char buf[64];
		sprintf(buf, "%p", obj.obj_.get());
		return variant(std::string(buf));
	DEFINE_FIELD(position, "[decimal,decimal,decimal]")
		const glm::vec3& v = obj.obj_->current.position;
		return vec3_to_variant(v);

	DEFINE_SET_FIELD
		obj.obj_->current.position = obj.obj_->initial.position = variant_to_vec3(value);
	
	DEFINE_FIELD(emission_rate, "any")
		return variant();
	DEFINE_SET_FIELD
		obj.obj_->setEmissionRate(value);
	END_DEFINE_CALLABLE(ParticleEmitterProxy)

	BEGIN_DEFINE_CALLABLE_NOBASE(ParticleAffectorProxy)
	DEFINE_FIELD(addr, "string")
		char buf[64];
		sprintf(buf, "%p", obj.obj_.get());
		return variant(std::string(buf));
	DEFINE_FIELD(node, "map")
		return obj.obj_->node();
	DEFINE_SET_FIELD
		obj.obj_->setNode(value);
	END_DEFINE_CALLABLE(ParticleAffectorProxy)

}

