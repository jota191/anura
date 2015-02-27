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
	   in a product, an acknowledgement in the product documentation would be
	   appreciated but is not required.

	   2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	   3. This notice may not be removed or altered from any source
	   distribution.
*/

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "asserts.hpp"
#include "AttributeSet.hpp"
#include "DisplayDevice.hpp"
#include "ShadersOGL.hpp"
#include "TextureOGL.hpp"
#include "UniformBufferOGL.hpp"

namespace KRE
{
	namespace OpenGL
	{
		namespace
		{
			struct uniform_mapping { const char* alt_name; const char* name; };
			struct attribute_mapping { const char* alt_name; const char* name; };

			const char* const default_vs = 
				"uniform mat4 u_mvp_matrix;\n"
				"attribute vec2 a_position;\n"
				"attribute vec2 a_texcoord;\n"
				"varying vec2 v_texcoord;\n"
				"void main()\n"
				"{\n"
				"    v_texcoord = a_texcoord;\n"
				"    gl_Position = u_mvp_matrix * vec4(a_position,0.0,1.0);\n"
				"}\n";
			const char* const default_fs =
				"uniform sampler2D u_tex_map;\n"
				"uniform sampler2D u_palette_map;\n"
				"uniform bool u_enable_palette_lookup;\n"
				"uniform float u_palette;\n"
				"uniform float u_palette_width;\n"
				"uniform bool u_discard;\n"
				"uniform vec4 u_color;\n"
				"varying vec2 v_texcoord;\n"
				"void main()\n"
				"{\n"
				"    vec4 color = texture2D(u_tex_map, v_texcoord);\n"
				"    if(u_enable_palette_lookup) {\n"
				"        color = texture2D(u_palette_map, vec2(255.0 * color.r / (u_palette_width-0.5), u_palette));\n"
				"    }\n"
				"    if(u_discard && color[3] == 0.0) {\n"
				"        discard;\n"
				"    } else {\n"
				"        gl_FragColor = color * u_color;\n"
				"    }\n"
				"}\n";

			const uniform_mapping default_uniform_mapping[] =
			{
				{"mvp_matrix", "u_mvp_matrix"},
				{"color", "u_color"},
				{"discard", "u_discard"},
				{"tex_map", "u_tex_map"},
				{"palette", "u_palette"},
				{"palette_width", "u_palette_width"},
				{"palette_map", "u_palette_map"},
				{"enable_palette_lookup", "u_enable_palette_lookup"},
				{"tex_map0", "u_tex_map"},
				{"", ""},
			};
			const attribute_mapping default_attribue_mapping[] =
			{
				{"position", "a_position"},
				{"texcoord", "a_texcoord"},
				{"", ""},
			};

			const char* const simple_vs = 
				"uniform mat4 u_mvp_matrix;\n"
				"uniform float u_point_size;\n"
				"attribute vec2 a_position;\n"
				"void main()\n"
				"{\n"
				"    gl_PointSize = u_point_size;\n"
				"    gl_Position = u_mvp_matrix * vec4(a_position, 0.0, 1.0);\n"
				"}\n";
			const char* const simple_fs =
				"uniform bool u_discard;\n"
				"uniform vec4 u_color;\n"
				"void main()\n"
				"{\n"
				"    gl_FragColor = u_color;\n"
				"    if(u_discard && gl_FragColor[3] == 0.0) {\n"
				"        discard;\n"
				"    }\n"
				"}\n";

			const uniform_mapping simple_uniform_mapping[] =
			{
				{"mvp_matrix", "u_mvp_matrix"},
				{"color", "u_color"},
				{"discard", "u_discard"},
				{"point_size", "u_point_size"},
				{"", ""},
			};
			const attribute_mapping simple_attribue_mapping[] =
			{
				{"position", "a_position"},
				{"", ""},
			};

			// circle shader definition starts
			const char* const circle_vs = 
				"uniform mat4 u_mvp_matrix;\n"
				"attribute vec2 a_position;\n"
				"varying vec2 v_position;\n"
				"void main()\n"
				"{\n"
				"	gl_Position = u_mvp_matrix * vec4(a_position, 0.0, 1.0);\n"
				"	v_position = a_position;\n"
				"}\n";
			const char* const circle_fs =
				"uniform bool u_discard;\n"
				"uniform vec4 u_color;\n"
				"uniform float u_outer_radius;\n"
				"uniform float u_inner_radius;\n"
				"uniform vec2 u_centre;\n"
				"uniform vec2 u_screen_dimensions;\n"
				"varying vec2 v_position;\n"
				"void main()\n"
				"{\n"
				// This adjusts for gl_FragCoord being origin at the bottom-left of the screen.
				"	vec2 pos = vec2(gl_FragCoord.x, u_screen_dimensions.y - gl_FragCoord.y) - u_centre;\n"
				"	float dist_squared = dot(pos, pos);\n"
				"	float r_squared = u_outer_radius*u_outer_radius;\n"
				"	if(u_inner_radius > 0.0f && dist_squared < u_inner_radius*u_inner_radius) {\n"
				"		gl_FragColor = mix(vec4(u_color.rgb, 0.0), u_color, smoothstep(u_inner_radius*u_inner_radius-u_inner_radius-0.25, u_inner_radius*u_inner_radius+u_inner_radius-0.25, dist_squared));\n"
				"	} else if(dist_squared < r_squared) {\n"
				"		gl_FragColor = mix(u_color, vec4(u_color.rgb, 0.0), smoothstep(r_squared-u_outer_radius+0.25, r_squared+u_outer_radius+0.25, dist_squared));\n"
				"	} else {\n"
				"		discard;\n"
				"	}\n"
				"}\n";

			const uniform_mapping circle_uniform_mapping[] =
			{
				{"mvp_matrix", "u_mvp_matrix"},
				{"color", "u_color"},
				{"discard", "u_discard"},
				{"outer_radius", "u_outer_radius"},
				{"inner_radius", "u_inner_radius"},
				{"screen_dimensions", "u_screen_dimensions"},
				{"centre", "u_centre"},
				{"", ""},
			};
			const attribute_mapping circle_attribue_mapping[] =
			{
				{"position", "a_position"},
				{"", ""},
			};
			// circle shader definition ends

			const char* const complex_vs = 
				"uniform mat4 u_mv_matrix;\n"
				"uniform mat4 u_p_matrix;\n"
				"uniform float u_point_size;\n"
				"uniform float u_line_width;\n"
				"attribute vec2 a_position;\n"
				"attribute vec2 a_normal;\n"
				"varying vec2 v_normal;\n"
				"void main()\n"
				"{\n"
				"    gl_PointSize = u_point_size;\n"
				"    vec4 delta = vec4(a_normal * u_line_width, 0.0, 0.0);\n"
				"    vec4 pos = u_mv_matrix * vec4(a_position, 0.0, 1.0);\n"
				"    gl_Position = u_p_matrix * (pos + delta);\n"
				"    v_normal = a_normal;\n"
				"}\n";
			const char* const complex_fs =
				"uniform bool u_discard;\n"
				"uniform vec4 u_color;\n"
				"uniform float u_line_width;\n"
				"uniform float u_blur;\n"
				"varying vec2 v_normal;\n"
				"void main()\n"
				"{\n"
				"    float blur = 2.0;\n"
				"    float dist = length(v_normal) * u_line_width;\n"
				"    float alpha = clamp((u_line_width - dist) / u_blur, 0.0, 1.0);\n"
				"    gl_FragColor = vec4(u_color.rgb, alpha);\n"
				"    if(u_discard && gl_FragColor[3] == 0.0) {\n"
				"        discard;\n"
				"    }\n"
				"}\n";

			const uniform_mapping complex_uniform_mapping[] =
			{
				{"mv_matrix", "u_mv_matrix"},
				{"p_matrix", "u_p_matrix"},
				{"color", "u_color"},
				{"discard", "u_discard"},
				{"point_size", "u_point_size"},
				{"line_width", "u_line_width"},
				{"", ""},
			};
			const attribute_mapping complex_attribue_mapping[] =
			{
				{"position", "a_position"},
				{"normal", "a_normal"},
				{"", ""},
			};

			const char* const attr_color_vs = 
				"uniform mat4 u_mvp_matrix;\n"
				"uniform float u_point_size;\n"
				"attribute vec2 a_position;\n"
				"attribute vec4 a_color;\n"
				"varying vec4 v_color;\n"
				"void main()\n"
				"{\n"
				"	 v_color = a_color;\n"
				"    gl_PointSize = u_point_size;\n"
				"    gl_Position = u_mvp_matrix * vec4(a_position,0.0,1.0);\n"
				"}\n";
			const char* const attr_color_fs =
				"uniform bool u_discard;\n"
				"uniform vec4 u_color;\n"
				"varying vec4 v_color;\n"
				"void main()\n"
				"{\n"
				"    gl_FragColor = v_color * u_color;\n"
				"    if(u_discard && gl_FragColor[3] == 0.0) {\n"
				"        discard;\n"
				"    }\n"
				"}\n";

			const uniform_mapping attr_color_uniform_mapping[] =
			{
				{"mvp_matrix", "u_mvp_matrix"},
				{"color", "u_color"},
				{"discard", "u_discard"},
				{"point_size", "u_point_size"},
				{"", ""},
			};
			const attribute_mapping attr_color_attribue_mapping[] =
			{
				{"position", "a_position"},
				{"color", "a_color"},
				{"", ""},
			};

			const char* const vtc_vs = 
				"uniform mat4 u_mvp_matrix;\n"
				"attribute vec2 a_position;\n"
				"attribute vec2 a_texcoord;\n"
				"attribute vec4 a_color;\n"
				"varying vec2 v_texcoord;\n"
				"varying vec4 v_color;\n"
				"void main()\n"
				"{\n"
				"    v_color = a_color;\n"
				"    v_texcoord = a_texcoord;\n"
				"    gl_Position = u_mvp_matrix * vec4(a_position,0.0,1.0);\n"
				"}\n";
			const char* const vtc_fs =
				"uniform sampler2D u_tex_map;\n"
				"varying vec2 v_texcoord;\n"
				"varying vec4 v_color;\n"
				"uniform vec4 u_color;\n"
				"void main()\n"
				"{\n"
				"    vec4 color = texture2D(u_tex_map, v_texcoord);\n"
				"    gl_FragColor = color * v_color * u_color;\n"
				"}\n";

			const uniform_mapping vtc_uniform_mapping[] =
			{
				{"mvp_matrix", "u_mvp_matrix"},
				{"color", "u_color"},
				{"tex_map", "u_tex_map"},
				{"tex_map0", "u_tex_map"},
				{"", ""},
			};
			const attribute_mapping vtc_attribue_mapping[] =
			{
				{"position", "a_position"},
				{"texcoord", "a_texcoord"},
				{"color", "a_color"},
				{"", ""},
			};

			const char* const point_shader_vs = 
				"uniform mat4 u_mvp_matrix;\n"
				"uniform float u_point_size;\n"
				"attribute vec2 a_position;\n"
				"void main()\n"
				"{\n"
				"    gl_PointSize = u_point_size;\n"
				"    gl_Position = u_mvp_matrix * vec4(a_position,0.0,1.0);\n"
				"}\n";
			const char* const point_shader_fs = 
				"uniform vec4 u_color;\n"
				"void main()\n"
				"{\n"
				"    gl_FragColor = u_color;\n"
				"}\n";
			const uniform_mapping point_shader_uniform_mapping[] = 
			{
				{"mvp_matrix", "u_mvp_matrix"},
				{"color", "u_color"},
				{"point_size", "u_point_size"},
				{"", ""},
			};
			const attribute_mapping point_shader_attribute_mapping[] = 
			{
				{"position", "a_position"},
				{"", ""},
			};

			const struct {
				const char* shader_name;
				const char* vertex_shader_name;
				const char* const vertex_shader_data;
				const char* fragment_shader_name;
				const char* const fragment_shader_data;
				const uniform_mapping* u_mapping;
				const attribute_mapping* a_mapping;
			} shader_defs[] = 
			{
				{ "default", "default_vs", default_vs, "default_fs", default_fs, default_uniform_mapping, default_attribue_mapping },
				{ "simple", "simple_vs", simple_vs, "simple_fs", simple_fs, simple_uniform_mapping, simple_attribue_mapping },
				{ "complex", "complex_vs", complex_vs, "complex_fs", complex_fs, complex_uniform_mapping, complex_attribue_mapping },
				{ "attr_color_shader", "attr_color_vs", attr_color_vs, "attr_color_fs", attr_color_fs, attr_color_uniform_mapping, attr_color_attribue_mapping },
				{ "vtc_shader", "vtc_vs", vtc_vs, "vtc_fs", vtc_fs, vtc_uniform_mapping, vtc_attribue_mapping },
				{ "circle", "circle_vs", circle_vs, "circle_fs", circle_fs, circle_uniform_mapping, circle_attribue_mapping },
				{ "point_shader", "point_shader_vs", point_shader_vs, "point_shader_fs", point_shader_fs, point_shader_uniform_mapping, point_shader_attribute_mapping },
			};

			typedef std::map<std::string, ShaderProgramPtr> shader_factory_map;
			shader_factory_map& get_shader_factory()
			{
				static shader_factory_map res;
				if(res.empty()) {
					// XXX load some default shaders here.
					for(auto& def : shader_defs) {
						
						auto spp = std::make_shared<OpenGL::ShaderProgram>(def.shader_name, 
							ShaderDef(def.vertex_shader_name, def.vertex_shader_data),
							ShaderDef(def.fragment_shader_name, def.fragment_shader_data),
							variant());
						res[def.shader_name] = spp;
						auto um = def.u_mapping;
						while(strlen(um->alt_name) > 0) {
							spp->setAlternateUniformName(um->name, um->alt_name);
							++um;
						}
						auto am = def.a_mapping;
						while(strlen(am->alt_name) > 0) {
							spp->setAlternateAttributeName(am->name, am->alt_name);
							++am;
						}
						spp->setActives();
					}
				}
				return res;
			}

			GLenum convert_render_variable_type(AttrFormat type)
			{
				switch(type) {
					case AttrFormat::BOOL:							return GL_BYTE;
					case AttrFormat::HALF_FLOAT:					return GL_HALF_FLOAT;
					case AttrFormat::FLOAT:							return GL_FLOAT;
					case AttrFormat::DOUBLE:						return GL_DOUBLE;
					case AttrFormat::FIXED:							return GL_FIXED;
					case AttrFormat::SHORT:							return GL_SHORT;
					case AttrFormat::UNSIGNED_SHORT:				return GL_UNSIGNED_SHORT;
					case AttrFormat::BYTE:							return GL_BYTE;
					case AttrFormat::UNSIGNED_BYTE:					return GL_UNSIGNED_BYTE;
					case AttrFormat::INT:							return GL_INT;
					case AttrFormat::UNSIGNED_INT:					return GL_UNSIGNED_INT;
					case AttrFormat::INT_2_10_10_10_REV:			return GL_INT_2_10_10_10_REV;
					case AttrFormat::UNSIGNED_INT_2_10_10_10_REV:	return GL_UNSIGNED_INT_2_10_10_10_REV;
					case AttrFormat::UNSIGNED_INT_10F_11F_11F_REV:	return GL_UNSIGNED_INT_10F_11F_11F_REV;
				}
				ASSERT_LOG(false, "Unrecognised value for variable type.");
				return GL_NONE;
			}
		}

		Shader::Shader(GLenum type, const std::string& name, const std::string& code)
			: type_(type), 
			  shader_(0), 
			  name_(name)
		{
			bool compiled_ok = compile(code);
			ASSERT_LOG(compiled_ok == true, "Error compiling shader for " << name_);
		}

		bool Shader::compile(const std::string& code)
		{
			GLint compiled;
			if(shader_) {
				glDeleteShader(shader_);
				shader_ = 0;
			}

			ASSERT_LOG(glCreateShader != nullptr, "Something bad happened with Glew shader not initialised.");
			shader_ = glCreateShader(type_);
			if(shader_ == 0) {
				std::cerr << "Enable to create shader." << std::endl;
				return false;
			}
			const char* shader_code = code.c_str();
			glShaderSource(shader_, 1, &shader_code, nullptr);
			glCompileShader(shader_);
			glGetShaderiv(shader_, GL_COMPILE_STATUS, &compiled);
			if(!compiled) {
				GLint info_len = 0;
				glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &info_len);
				if(info_len > 1) {
					std::vector<char> info_log;
					info_log.resize(info_len);
					glGetShaderInfoLog(shader_, info_log.capacity(), nullptr, &info_log[0]);
					std::string s(info_log.begin(), info_log.end());
					std::cerr << "Error compiling shader: " << s << std::endl;
				}
				glDeleteShader(shader_);
				shader_ = 0;
				return false;
			}
			return true;
		}

		ShaderProgram::ShaderProgram(const std::string& name, const ShaderDef& vs, const ShaderDef& fs, const variant& node)
			: KRE::ShaderProgram(node),
			  object_(0)
		{
			init(name, vs, fs);
		}

		ShaderProgram::~ShaderProgram()
		{
			if(object_ != 0) {
				glDeleteShader(object_);
				object_ = 0;
			}
		}

		void ShaderProgram::init(const std::string& name, const ShaderDef& vs, const ShaderDef& fs)
		{
			name_ = name;
			vs_.reset(new Shader(GL_VERTEX_SHADER, vs.first, vs.second));
			fs_.reset(new Shader(GL_FRAGMENT_SHADER, fs.first, fs.second));
			bool linked_ok = link();
			ASSERT_LOG(linked_ok == true, "Error linking program: " << name_);
		}

		int ShaderProgram::getAttributeOrDie(const std::string& attr) const
		{
			int attr_value = getAttribute(attr);
			ASSERT_LOG(attr_value != ShaderProgram::INALID_ATTRIBUTE, "Could not find attribute '" << attr << "' in shader: " << name());
			return attr_value;
		}

		int ShaderProgram::getUniformOrDie(const std::string& attr) const
		{
			int uniform_value = getUniform(attr);
			ASSERT_LOG(uniform_value != ShaderProgram::INALID_UNIFORM, "Could not find uniform '" << attr << "' in shader: " << name());
			return uniform_value;
		}

		int ShaderProgram::getAttribute(const std::string& attr) const
		{
			auto it = attribs_.find(attr);
			if(it != attribs_.end()) {
				return it->second.location;
			}
			auto alt_name_it = attribute_alternate_name_map_.find(attr);
			if(alt_name_it == attribute_alternate_name_map_.end()) {
				LOG_WARN("Attribute '" << attr << "' not found in alternate names list and is not a name defined in the shader: " << name_);
				return ShaderProgram::INALID_ATTRIBUTE;
			}
			it = attribs_.find(alt_name_it->second);
			if(it == attribs_.end()) {
				LOG_WARN("Attribute \"" << alt_name_it->second << "\" not found in list, looked up from symbol " << attr << " in shader: " << name_);
				return ShaderProgram::INALID_ATTRIBUTE;
			}
			return it->second.location;
		}

		int ShaderProgram::getUniform(const std::string& attr) const
		{
			auto it = uniforms_.find(attr);
			if(it != uniforms_.end()) {
				return it->second.location;
			}
			auto alt_name_it = uniform_alternate_name_map_.find(attr);
			if(alt_name_it == uniform_alternate_name_map_.end()) {
				//LOG_WARN("Uniform '" << attr << "' not found in alternate names list and is not a name defined in the shader: " << name_);
				return ShaderProgram::INALID_UNIFORM;
			}
			it = uniforms_.find(alt_name_it->second);
			if(it == uniforms_.end()) {
				//LOG_WARN("Uniform \"" << alt_name_it->second << "\" not found in list, looked up from symbol " << attr << " in shader: " << name_);
				return ShaderProgram::INALID_UNIFORM;
			}
			return it->second.location;
		}

		bool ShaderProgram::link()
		{
			if(object_) {
				glDeleteProgram(object_);
				object_ = 0;
			}
			object_ = glCreateProgram();
			ASSERT_LOG(object_ != 0, "Unable to create program object.");
			glAttachShader(object_, vs_->get());
			glAttachShader(object_, fs_->get());
			glLinkProgram(object_);
			GLint linked = 0;
			glGetProgramiv(object_, GL_LINK_STATUS, &linked);
			if(!linked) {
				GLint info_len = 0;
				glGetProgramiv(object_, GL_INFO_LOG_LENGTH, &info_len);
				if(info_len > 1) {
					std::vector<char> info_log;
					info_log.resize(info_len);
					glGetProgramInfoLog(object_, info_log.capacity(), nullptr, &info_log[0]);
					std::string s(info_log.begin(), info_log.end());
					std::cerr << "Error linking object: " << s << std::endl;
				}
				glDeleteProgram(object_);
				object_ = 0;
				return false;
			}
			return queryUniforms() && queryAttributes();
		}

		bool ShaderProgram::queryUniforms()
		{
			GLint active_uniforms;
			glGetProgramiv(object_, GL_ACTIVE_UNIFORMS, &active_uniforms);
			GLint uniform_max_len;
			glGetProgramiv(object_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_max_len);
			std::vector<char> name;
			name.resize(uniform_max_len+1);
			for(int i = 0; i < active_uniforms; i++) {
				Actives u;
				GLsizei size;
				glGetActiveUniform(object_, i, name.size(), &size, &u.num_elements, &u.type, &name[0]);
				u.name = std::string(&name[0], &name[size]);
				u.location = glGetUniformLocation(object_, u.name.c_str());
				ASSERT_LOG(u.location >= 0, "Unable to determine the location of the uniform: " << u.name);
				uniforms_[u.name] = u;
				v_uniforms_[u.location] = u;
			}
			return true;
		}

		bool ShaderProgram::queryAttributes()
		{
			GLint active_attribs;
			glGetProgramiv(object_, GL_ACTIVE_ATTRIBUTES, &active_attribs);
			GLint attributes_max_len;
			glGetProgramiv(object_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributes_max_len);
			std::vector<char> name;
			name.resize(attributes_max_len+1);
			for(int i = 0; i < active_attribs; i++) {
				Actives a;
				GLsizei size;
				glGetActiveAttrib(object_, i, name.size(), &size, &a.num_elements, &a.type, &name[0]);
				a.name = std::string(&name[0], &name[size]);
				a.location = glGetAttribLocation(object_, a.name.c_str());
				ASSERT_LOG(a.location >= 0, "Unable to determine the location of the attribute: " << a.name);
				ASSERT_LOG(a.num_elements == 1, "More than one element was found for an attribute(" << a.name << ") in shader(" << this->name() << "): " << a.num_elements);
				attribs_[a.name] = a;
				v_attribs_[a.location] = a;
			}
			return true;
		}

		void ShaderProgram::makeActive()
		{
			glUseProgram(object_);
		}

		void ShaderProgram::setUniformValue(int uid, const void* value) const
		{
			if(uid == ShaderProgram::INALID_UNIFORM) {
				LOG_WARN("Tried to set value for invalid uniform iterator.");
				return;
			}
			auto it = v_uniforms_.find(uid);
			ASSERT_LOG(it != v_uniforms_.end(), "Couldn't find location " << uid << " on the uniform list.");
			const Actives& u = it->second;
			ASSERT_LOG(value != nullptr, "setUniformValue(): value is nullptr");
			switch(u.type) {
			case GL_INT:
			case GL_BOOL:
			case GL_SAMPLER_2D:
			case GL_SAMPLER_CUBE:	
				glUniform1i(u.location, *(GLint*)value); 
				break;
			case GL_INT_VEC2:	
			case GL_BOOL_VEC2:	
				glUniform2i(u.location, ((GLint*)value)[0], ((GLint*)value)[1]); 
				break;
			case GL_INT_VEC3:	
			case GL_BOOL_VEC3:	
				glUniform3iv(u.location, u.num_elements, (GLint*)value); 
				break;
			case GL_INT_VEC4: 	
			case GL_BOOL_VEC4:
				glUniform4iv(u.location, u.num_elements, (GLint*)value); 
				break;

			case GL_FLOAT: {
				glUniform1f(u.location, *(GLfloat*)value);
				break;
			}
			case GL_FLOAT_VEC2: {
				glUniform2fv(u.location, u.num_elements, (GLfloat*)value);
				break;
			}
			case GL_FLOAT_VEC3: {
				glUniform3fv(u.location, u.num_elements, (GLfloat*)value);
				break;
			}
			case GL_FLOAT_VEC4: {
				glUniform4fv(u.location, u.num_elements, (GLfloat*)value);
				break;
			}
			case GL_FLOAT_MAT2:	{
				glUniformMatrix2fv(u.location, u.num_elements, GL_FALSE, (GLfloat*)value);
				break;
			}
			case GL_FLOAT_MAT3: {
				glUniformMatrix3fv(u.location, u.num_elements, GL_FALSE, (GLfloat*)value);
				break;
			}
			case GL_FLOAT_MAT4: {
				glUniformMatrix4fv(u.location, u.num_elements, GL_FALSE, (GLfloat*)value);
				break;
			}
			default:
				ASSERT_LOG(false, "Unhandled uniform type: " << it->second.type);
			}
		}

		void ShaderProgram::setUniformValue(int uid, const GLint value) const
		{
			if(uid == ShaderProgram::INALID_UNIFORM) {
				LOG_WARN("Tried to set value for invalid uniform iterator.");
				return;
			}
			auto it = v_uniforms_.find(uid);
			ASSERT_LOG(it != v_uniforms_.end(), "Couldn't find location " << uid << " on the uniform list.");
			const Actives& u = it->second;
			switch(u.type) {
			case GL_INT:
			case GL_BOOL:
			case GL_SAMPLER_2D:
			case GL_SAMPLER_CUBE:	
				glUniform1i(u.location, value); 
				break;
			default:
				ASSERT_LOG(false, "Unhandled uniform type: " << it->second.type);
			}
		}

		void ShaderProgram::setUniformValue(int uid, const GLfloat value) const
		{
			if(uid == ShaderProgram::INALID_UNIFORM) {
				LOG_WARN("Tried to set value for invalid uniform iterator.");
				return;
			}
			auto it = v_uniforms_.find(uid);
			ASSERT_LOG(it != v_uniforms_.end(), "Couldn't find location " << uid << " on the uniform list.");
			const Actives& u = it->second;
			switch(u.type) {
			case GL_FLOAT: {
				glUniform1f(u.location, value);
				break;
			}
			default:
				ASSERT_LOG(false, "Unhandled uniform type: " << it->second.type);
			}	
		}

		void ShaderProgram::setUniformValue(int uid, const GLint* value) const
		{
			if(uid == ShaderProgram::INALID_UNIFORM) {
				LOG_WARN("Tried to set value for invalid uniform iterator.");
				return;
			}
			auto it = v_uniforms_.find(uid);
			ASSERT_LOG(it != v_uniforms_.end(), "Couldn't find location " << uid << " on the uniform list.");
			const Actives& u = it->second;
			ASSERT_LOG(value != nullptr, "set_uniform(): value is nullptr");
			switch(u.type) {
			case GL_INT:
			case GL_BOOL:
			case GL_SAMPLER_2D:
			case GL_SAMPLER_CUBE:	
				glUniform1i(u.location, *value); 
				break;
			case GL_INT_VEC2:	
			case GL_BOOL_VEC2:	
				glUniform2i(u.location, value[0], value[1]); 
				break;
			case GL_INT_VEC3:	
			case GL_BOOL_VEC3:	
				glUniform3iv(u.location, u.num_elements, value); 
				break;
			case GL_INT_VEC4: 	
			case GL_BOOL_VEC4:
				glUniform4iv(u.location, u.num_elements, value); 
				break;
			default:
				ASSERT_LOG(false, "Unhandled uniform type: " << it->second.type);
			}
		}

		void ShaderProgram::setUniformValue(int uid, const GLfloat* value) const
		{
			if(uid == ShaderProgram::INALID_UNIFORM) {
				LOG_WARN("Tried to set value for invalid uniform iterator.");
				return;
			}
			auto it = v_uniforms_.find(uid);
			ASSERT_LOG(it != v_uniforms_.end(), "Couldn't find location " << uid << " on the uniform list.");
			const Actives& u = it->second;
			ASSERT_LOG(value != nullptr, "setUniformValue(): value is nullptr");
			switch(u.type) {
			case GL_FLOAT: {
				glUniform1f(u.location, *value);
				break;
			}
			case GL_FLOAT_VEC2: {
				glUniform2fv(u.location, u.num_elements, value);
				break;
			}
			case GL_FLOAT_VEC3: {
				glUniform3fv(u.location, u.num_elements, value);
				break;
			}
			case GL_FLOAT_VEC4: {
				glUniform4fv(u.location, u.num_elements, value);
				break;
			}
			case GL_FLOAT_MAT2:	{
				glUniformMatrix2fv(u.location, u.num_elements, GL_FALSE, value);
				break;
			}
			case GL_FLOAT_MAT3: {
				glUniformMatrix3fv(u.location, u.num_elements, GL_FALSE, value);
				break;
			}
			case GL_FLOAT_MAT4: {
				glUniformMatrix4fv(u.location, u.num_elements, GL_FALSE, value);
				break;
			}
			default:
				ASSERT_LOG(false, "Unhandled uniform type: " << it->second.type);
			}	
		}

		void ShaderProgram::setAlternateUniformName(const std::string& name, const std::string& alt_name)
		{
			ASSERT_LOG(uniform_alternate_name_map_.find(alt_name) == uniform_alternate_name_map_.end(),
				"Trying to replace alternative uniform name: " << alt_name << " " << name);
			uniform_alternate_name_map_[alt_name] = name;
		}

		void ShaderProgram::setAlternateAttributeName(const std::string& name, const std::string& alt_name)
		{
			ASSERT_LOG(attribute_alternate_name_map_.find(alt_name) == attribute_alternate_name_map_.end(),
				"Trying to replace alternative attribute name: " << alt_name << " " << name);
			attribute_alternate_name_map_[alt_name] = name;
		}

		void ShaderProgram::setActives()
		{
			glUseProgram(object_);
			// Cache some frequently used uniforms.
			u_mvp_ = getUniform("mvp_matrix");
			u_mv_ = getUniform("mv_matrix");
			u_p_ = getUniform("p_matrix");
			u_color_ = getUniform("color");
			u_line_width_ = getUniform("line_width");
			u_tex_ = getUniform("tex_map");
			a_vertex_ = getAttribute("position");
			a_texcoord_ = getAttribute("texcoord");
			a_color_ = getAttribute("a_color");
			a_normal_ = getAttribute("normal");

			// I don't like having to have these as well.
			u_enable_palette_lookup_ = getUniform("enable_palette_lookup");
			u_palette_ = getUniform("palette");
			u_palette_width_ = getUniform("u_palette_width");
			u_palette_map_ = getUniform("palette_map");
		}

		ShaderProgramPtr ShaderProgram::factory(const std::string& name)
		{
			auto& sf = get_shader_factory();
			auto it = sf.find(name);
			ASSERT_LOG(it != sf.end(), "Shader '" << name << "' not found in the list of shaders.");
			return it->second;
		}

		ShaderProgramPtr ShaderProgram::factory(const variant& node)
		{
			return getProgramFromVariant(node);
		}

		ShaderProgramPtr ShaderProgram::defaultSystemShader()
		{
			auto& sf = get_shader_factory();
			auto it = sf.find("default");
			ASSERT_LOG(it != sf.end(), "No 'default' shader found in the list of shaders.");
			return it->second;
		}

		ShaderProgramPtr ShaderProgram::getProgramFromVariant(const variant& node)
		{
			auto& sf = get_shader_factory();

			ASSERT_LOG(node.is_map(), "instance must be a map.");
			ASSERT_LOG(node.has_key("fragment") && node.has_key("vertex") && node.has_key("name"), 
				"instances must have 'fragment', 'vertex' and 'name' attributes.");
		
			const std::string& name = node["name"].as_string();
			const std::string& vert_data = node["vertex"].as_string();
			const std::string& frag_data = node["fragment"].as_string();

			auto it = sf.find(name);
			if(it != sf.end()) {
				return it->second;
			}

			auto spp = std::make_shared<OpenGL::ShaderProgram>(name, 
				ShaderDef(name + "_vs", vert_data),
				ShaderDef(name + "_fs", frag_data),
				node);
			it = sf.find(name);
			if(it != sf.end()) {
				LOG_WARN("Overwriting shader with name: " << name);
			}
			sf[name] = spp;
			if(node.has_key("uniforms")) {
				ASSERT_LOG(node["uniforms"].is_map(), "'uniforms' attribute in shader(" << name << ") must be a map.");
				for(auto uni : node["uniforms"].as_map()) {
					spp->setAlternateUniformName(uni.second.as_string(), uni.first.as_string());
				}
			}
			if(node.has_key("attributes")) {
				ASSERT_LOG(node["attributes"].is_map(), "'attributes' attribute in shader(" << name << ") must be a map.");
				for(auto attr : node["attributes"].as_map()) {
					spp->setAlternateAttributeName(attr.second.as_string(), attr.first.as_string());
				}
			}
			spp->setActives();

			return spp;
		}

		void ShaderProgram::loadShadersFromVariant(const variant& node)
		{
			ASSERT_LOG(node.has_key("instances"), "Shader data must have 'instances' attribute.");
			ASSERT_LOG(node["instances"].is_list(), "'instances' attribute should be a list.");

			if(node.has_key("instances") && node["instances"].is_list()) {
				for(auto instance : node["instances"].as_list()) {
					getProgramFromVariant(instance);
				}
			} else {
				getProgramFromVariant(node);
			}
		}

		void ShaderProgram::configureActives(AttributeSetPtr attrset)
		{
			for(auto& attr : attrset->getAttributes()) {
				for(auto& desc : attr->getAttrDesc()) {
					desc.setLocation(getAttribute(desc.getAttrName()));
				}
			}
		}
		
		void ShaderProgram::configureUniforms(UniformBufferBase& uniforms)
		{
			/*if(DisplayDevice::checkForFeature(DisplayDeviceCapabilties::UNIFORM_BUFFERS)) {
				auto hw = std::unique_ptr<UniformHardwareOGL>(new UniformHardwareOGL(uniforms.getName()));
				uniforms.setHardware(std::move(hw));
			}*/
		}

		void ShaderProgram::applyAttribute(AttributeBasePtr attr) 
		{
			auto attr_hw = attr->getDeviceBufferData();
			attr_hw->bind();
			for(auto& attrdesc : attr->getAttrDesc()) {
				auto loc = attrdesc.getLocation();
				glEnableVertexAttribArray(loc);					
				glVertexAttribPointer(loc, 
					attrdesc.getNumElements(), 
					convert_render_variable_type(attrdesc.getVarType()), 
					attrdesc.normalise(), 
					attrdesc.getStride(), 
					reinterpret_cast<const GLvoid*>(attr_hw->value() + attr->getOffset() + attrdesc.getOffset()));
				enabled_attribs_.emplace_back(loc);
			}
		}

		void ShaderProgram::cleanUpAfterDraw()
		{
			for(auto attrib : enabled_attribs_) {
				glDisableVertexAttribArray(attrib);
			}
			enabled_attribs_.clear();
		}

		void ShaderProgram::setUniformsForTexture(const TexturePtr& tex) const
		{
			if(tex) {
				// XXX The material may need to set more texture uniforms for multi-texture -- need to do that here.
				// Or maybe it should be done through the uniform block and override this somehow.
				if(getTexMapUniform() != ShaderProgram::INALID_UNIFORM) {
					setUniformValue(getTexMapUniform(), 0);
				}

				tex->bind();

				bool enable_palette = tex->isPaletteized();
				if(enable_palette) {
					if(u_palette_map_ != ShaderProgram::INALID_UNIFORM) {
						setUniformValue(u_palette_map_, 1);
					} else {
						enable_palette = false;
					}
					if(u_palette_ != ShaderProgram::INALID_UNIFORM) {
						// XXX replace tex->getSurfaces()[1]->height() with tex->getNormalizedCoordH(1, 1);
						float h = static_cast<float>(tex->getSurfaces()[1]->height() - 1);
						const float palette_sel = static_cast<float>(tex->getPalette()) / h;
						setUniformValue(u_palette_, palette_sel); 
					} else {
						enable_palette = false;
					}
					if(u_palette_width_ != ShaderProgram::INALID_UNIFORM) {
						// XXX this needs adjusted for pot texture width.
						setUniformValue(u_palette_width_, static_cast<float>(tex->getSurfaces()[1]->width()));
					} else {
						enable_palette = false;
					}
				}

				if(u_enable_palette_lookup_ != ShaderProgram::INALID_UNIFORM) {
					setUniformValue(u_enable_palette_lookup_, enable_palette);
				}
			}
		}
	}
}
