#pragma once

#include "gui/SimpleSpriteButton.h"
#include "TextTools.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class ImageButtonSprite: public SimpleSpriteButton
	{
	public:	

		ImageButtonSprite(Texture image, Vec2f pos)
			:SimpleSpriteButton(image.getWidth(), image.getHeight(), pos), image(image)
		{
		}

		void drawLayout()
		{
			gl::color(Color::white());	
			gl::color(color);		
			gl::draw(image, Vec2f(0, 3));		
			gl::color(Color::white());	
			//gl::drawSolidRect(buttonArea);
		}

		void changeTexture(Texture image)
		{
			this->image = image;
		}

	private:
		Texture image;	
	};

	typedef shared_ptr<ImageButtonSprite> ImageButtonSpriteRef;
}