#pragma once
#pragma once

#include "gui/SimpleButton.h"
#include "TextTools.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class ImageButton: public SimpleButton
	{
	public:	

		ImageButton(Texture image, Vec2f pos)
			:SimpleButton(image.getWidth(), image.getHeight(), pos), image(image)
		{
		}

		void drawLayout()
		{
			gl::color(Color::white());	
			gl::color(color);		
			gl::draw(image);		
			gl::color(Color::white());		
		}

		void changeTexture(Texture image)
		{
			this->image = image;
		}

	private:
		Texture image;	
	};

	typedef shared_ptr<ImageButton> ImageButtonRef;
}