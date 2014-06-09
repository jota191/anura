/*
	Copyright (C) 2003-2014 by David White <davewx7@gmail.com>
	
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

#include "kre/DisplayDevice.hpp"

#include "data_blob.hpp"
#include "image_widget.hpp"

#include <iostream>

namespace gui 
{
	ImageWidget::ImageWidget(const std::string& fname, int w, int h)
	  : texture_(KRE::DisplayDevice::GetCurrent()->CreateTexture(fname)), 
	  rotate_(0.0f), 
	  image_name_(fname)
	{
		setEnvironment();
		init(w, h);
	}

	ImageWidget::ImageWidget(KRE::TexturePtr tex, int w, int h)
	  : texture_(tex), 
	  rotate_(0.0)
	{
		setEnvironment();
		init(w, h);
	}

	ImageWidget::ImageWidget(const variant& v, game_logic::FormulaCallable* e) 
		: Widget(v, e)
	{
		if(v.has_key("area")) {
			area_ = rect(v["area"]);
		}

		if(v["image"].is_string()) {
			image_name_ = v["image"].as_string();
			texture_ = KRE::DisplayDevice::GetCurrent()->CreateTexture(image_name_);
		} else if(v["image"].is_callable()) {
			data_blob_ptr image_blob = data_blob_ptr(v["image"].try_convert<data_blob>());
			ASSERT_LOG(image_blob != NULL, "Couldn't convert callable in 'image' attribute into a data_blob.");
			image_name_ = (*image_blob)();
			texture_ = KRE::DisplayDevice::GetCurrent()->CreateTexture(image_name_);
		}

		rotate_ = v.has_key("rotation") ? v["rotation"].as_float() : 0.0f;
		init(v["image_width"].as_int(-1), v["image_height"].as_int(-1));

		setClaimMouseEvents(v["claim_mouse_events"].as_bool(false));
	}

	void ImageWidget::init(int w, int h)
	{
		if(w < 0) {
			if(area_.w()) {
				w = area_.w()*2;
			} else {
				w = texture_->Width();
			}
		}

		if(h < 0) {
			if(area_.h()) {
				h = area_.h()*2;
			} else {
				h = texture_->Height();
			}
		}

		setDim(w,h);
	}

	void ImageWidget::handleDraw() const
	{
		if(area_.w() == 0) {
			KRE::Canvas::getInstance()->blitTexture(texture_, rotate_, rect(x(), y(), width(), height()));
		} else {
			KRE::Canvas::getInstance()->blitTexture(texture_, area_, rotate_, rect(x(), y(), width(), height()));
		}
	}

	BEGIN_DEFINE_CALLABLE(ImageWidget, Widget)
		DEFINE_FIELD(image, "string")
			return variant(obj.image_name_);
		DEFINE_SET_FIELD_TYPE("string|callable")
			if(value.is_string()) {
				obj.image_name_ = value.as_string();
				obj.texture_ = KRE::DisplayDevice::GetCurrent()->CreateTexture(obj.image_name_);
			} else if(value.is_callable()) {
				data_blob_ptr image_blob = data_blob_ptr(value.try_convert<data_blob>());
				ASSERT_LOG(image_blob != NULL, "Couldn't convert callable in 'image' attribute into a data_blob.");
				obj.image_name_ = (*image_blob)();
				obj.texture_ = KRE::DisplayDevice::GetCurrent()->CreateTexture(obj.image_name_);
			}
			
		DEFINE_FIELD(area, "[int,int,int,int]")
			return obj.area_.write();
		DEFINE_SET_FIELD
			obj.area_ = rect(value);

		DEFINE_FIELD(rotation, "decimal")
			return variant(obj.rotate_);
		DEFINE_SET_FIELD
			obj.rotate_ = value.as_float();

		DEFINE_FIELD(width, "int")
			return variant(obj.texture_->Width());
		DEFINE_FIELD(height, "int")
			return variant(obj.texture_->Height());
		DEFINE_FIELD(image_width, "int")
			return variant(obj.texture_->Width());
		DEFINE_FIELD(image_height, "int")
			return variant(obj.texture_->Height());

		DEFINE_FIELD(image_wh, "[int,int]")
			std::vector<variant> v;
			v.emplace_back(variant(obj.area_.w()));
			v.emplace_back(variant(obj.area_.h()));
			return variant(&v);
		DEFINE_SET_FIELD
			obj.init(value[0].as_int(), value[1].as_int());
	END_DEFINE_CALLABLE(ImageWidget)

	GuiSectionWidget::GuiSectionWidget(const std::string& id, int w, int h, int scale)
	  : section_(GuiSection::get(id)), scale_(scale)
	{
		setEnvironment();
		if(section_ && w == -1) {
			setDim((section_->width()/2)*scale_, (section_->height()/2)*scale_);
		} else {
			setDim(w,h);
		}
	}

	GuiSectionWidget::GuiSectionWidget(const variant& v, game_logic::FormulaCallable* e) 
		: Widget(v,e)
	{
		section_ = GuiSection::get(v);
		scale_ = v["scale"].as_int(1);
		if(!v.has_key("width") && section_) {
			setDim((section_->width()/2)*scale_, (section_->height()/2)*scale_);
		}
	}

	void GuiSectionWidget::setGuiSection(const std::string& id)
	{
		section_ = GuiSection::get(id);
	}

	void GuiSectionWidget::handleDraw() const
	{
		if(section_) {
			section_->blit(x(), y(), width(), height());
		}
	}

	BEGIN_DEFINE_CALLABLE(GuiSectionWidget, Widget)
		DEFINE_FIELD(name, "string")
			return variant();
		DEFINE_SET_FIELD
			obj.setGuiSection(value.as_string());

		DEFINE_FIELD(scale, "decimal")
			return variant(obj.scale_);
		DEFINE_SET_FIELD
			if(obj.section_) {
				obj.setDim((obj.section_->width()/2)*obj.scale_, (obj.section_->height()/2)*obj.scale_);
			}
	END_DEFINE_CALLABLE(GuiSectionWidget)
}
