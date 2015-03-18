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

		ImageButtonSprite(ci::gl::Texture image, ci::Vec2f pos = Vec2f::zero())
			:SimpleSpriteButton(image.getWidth(), image.getHeight(), pos),
			image(image)
		{
		}

		void drawLayout()
		{
			gl::color(ci::Color::white());	
			gl::color(ColorA(color.r, color.g, color.b, alpha));
			gl::translate(animPosition);
			gl::draw(image, ci::Vec2f(0.0f, 3.0f));		
			gl::color(ci::Color::white());	
			//gl::drawSolidRect(buttonArea);
		}

		void changeTexture(ci::gl::Texture image)
		{
			this->image = image;
		}

		

	private:
		ci::gl::Texture image;	
		
		
	};

	typedef std::shared_ptr<ImageButtonSprite> ImageButtonSpriteRef;
}