#pragma once

#include "LoadButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class DecorLoadButton> DecorLoadButtonRef;

		class DecorLoadButton: public LoadButton
		{
		public:
			DecorLoadButton(ci::Rectf rect, TextItem textItem, ci::gl::Texture icon)
				:LoadButton(rect, textItem, icon)
			{

			}

			virtual void drawLayout()
			{	
				LoadButton::drawLayout();

				gl::pushMatrices();
				gl::translate(0, -200);
				gl::color(Color::hex(0xffff00));
				gl::lineWidth(7);
				gl::drawStrokedRoundedRect(ci::Rectf(ci::Vec2f::zero(), Vec2f(200,200)), 6);
				gl::lineWidth(1);	
				gl::popMatrices();
			}

			void draw(float x, float y)
			{
				LoadButton::draw(x, y);
			}

			virtual void draw()
			{					
				LoadButton::draw();
			}
	
		};	
	}
}