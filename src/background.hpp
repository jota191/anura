/*
	Copyright (C) 2003-2013 by David White <davewx7@gmail.com>
	
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

#pragma once

#include <stdint.h>

#include <string>
#include <vector>

#include "AttributeSet.hpp"
#include "geometry.hpp"
#include "SceneObject.hpp"
#include "Texture.hpp"
#include "variant.hpp"

//class which represents the background to a level.
class Background
{
public:
	static void loadModifiedBackgrounds();

	//gets a background associated with a given ID.
	static std::shared_ptr<Background> get(const std::string& id, int palette_id);

	//all available backgrounds.
	static std::vector<std::string> getAvailableBackgrounds();

	Background(variant node, int palette);
	const std::string& id() const { return id_; }
	variant write() const;
	void draw(int x, int y, const rect& area, const std::vector<rect>& opaque_areas, float rotation, int cycle) const;
	void drawForeground(int x, int y, float rotation, int cycle) const;

	void setOffset(const point& offset);
private:

	void drawLayers(int x, int y, const rect& area, const std::vector<rect>& opaque_areas, float rotation, int cycle) const;
	std::string id_, file_;
	KRE::Color top_, bot_;
	int width_, height_;
	point offset_;

	struct Layer : public KRE::SceneObject {
		Layer() : KRE::SceneObject("Background::Layer") {}
		std::string image;
		std::string image_formula;
		mutable KRE::TexturePtr texture;
		int xscale, yscale_top, yscale_bot;		//scales are how quickly the background scrolls compared to normal ground movement when the player
								//walks around.  They give us the illusion of 'depth'. 100 is normal ground, less=distant, more=closer
		
		int xspeed;				//speed is how fast (in millipixels/cycle) the bg moves on its own.  It's for drifting clounds/rivers.
		int xpad;               //amount of empty space padding we put between
		int scale;				//a multiplier on the dimensions of the image.  Usually unused.
		int xoffset;
		int yoffset;			
		KRE::Color color;

		KRE::ColorPtr color_above, color_below;
		
		// Top and bottom edges of the background.
		mutable int y1, y2;

		//if true, this layer is actually drawn in the foreground.
		bool foreground;

		//if false we can disable blending while this is drawn
		bool blend;

		//if true prevents the image being tiled.
		bool notile;

		bool tile_upwards, tile_downwards;

		std::shared_ptr<KRE::Attribute<KRE::vertex_texcoord>> attr_;
	};

	void drawLayer(int x, int y, const rect& area, float rotation, const Layer& bg, int cycle) const;

	std::vector<Layer> layers_;
	int palette_;
};
