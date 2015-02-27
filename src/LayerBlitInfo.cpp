#include "AttributeSet.hpp"
#include "DisplayDevice.hpp"
#include "LayerBlitInfo.hpp"

LayerBlitInfo::LayerBlitInfo()
	: KRE::SceneObject("layer_blit_info"),
	  xbase_(0),
	  ybase_(0),
	  initialised_(false)
{
	using namespace KRE;

	auto ab = DisplayDevice::createAttributeSet(true, false, false);
	opaques_ = std::make_shared<Attribute<tile_corner>>(AccessFreqHint::DYNAMIC, AccessTypeHint::DRAW);
	opaques_->addAttributeDesc(AttributeDesc(AttrType::POSITION, 2, AttrFormat::UNSIGNED_SHORT, false, sizeof(tile_corner), offsetof(tile_corner, vertex)));
	opaques_->addAttributeDesc(AttributeDesc(AttrType::TEXTURE, 2, AttrFormat::FLOAT, false, sizeof(tile_corner), offsetof(tile_corner, uv)));
	ab->addAttribute(opaques_);	
	ab->setDrawMode(DrawMode::TRIANGLES);
	addAttributeSet(ab);

	auto tab = DisplayDevice::createAttributeSet(true, false, false);
	transparent_ = std::make_shared<Attribute<tile_corner>>(AccessFreqHint::DYNAMIC, AccessTypeHint::DRAW);
	transparent_->addAttributeDesc(AttributeDesc(AttrType::POSITION, 2, AttrFormat::UNSIGNED_SHORT, false, sizeof(tile_corner), offsetof(tile_corner, vertex)));
	transparent_->addAttributeDesc(AttributeDesc(AttrType::TEXTURE, 2, AttrFormat::FLOAT, false, sizeof(tile_corner), offsetof(tile_corner, uv)));
	tab->addAttribute(transparent_);

	tab->setDrawMode(DrawMode::TRIANGLES);
	addAttributeSet(tab);
}

void LayerBlitInfo::addTextureToList(KRE::TexturePtr tex)
{
	tex_list_.emplace_back(tex);
}

void LayerBlitInfo::setVertices(std::vector<tile_corner>* op, std::vector<tile_corner>* tr)
{
	//LOG_DEBUG("Adding " << op->size() << " opqaue vertices and " << tr->size() << " transparent vertices.");
	getAttributeSet()[0]->setCount(getAttributeSet()[0]->getCount() + op->size());
	opaques_->update(op, opaques_->end());
	getAttributeSet()[1]->setCount(getAttributeSet()[1]->getCount() + tr->size());
	transparent_->update(tr, transparent_->end());
}
