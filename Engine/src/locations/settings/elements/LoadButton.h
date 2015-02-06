#pragma once

#include "TextTools.h"
#include "gui/SimpleButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class LoadButton> LoadButtonRef;

		class LoadButton: public SimpleButton
		{
		public:
			LoadButton(ci::Rectf rect, TextItem textItem, ci::gl::Texture icon)
				:SimpleButton(rect), textItem(textItem), icon(icon), bckColor(Color::hex(0x242135))
			{
				tex = textTools().getTextField(textItem);
				texPosX = 0.5 * (buttonArea.getWidth() - (tex.getWidth() + icon.getWidth() + 15.0f));
			}

			virtual void drawLayout()
			{	
				gl::color(bckColor);
				gl::drawSolidRoundedRect(buttonArea, 7);
				gl::color(Color::white());				

				gl::pushMatrices();
				gl::translate(texPosX, 23);				
				gl::draw(tex);
				gl::translate(tex.getWidth() + 15.0f, 0);
				gl::translate(0, -3);	
				gl::draw(icon);	
				gl::popMatrices();
			}

			void draw(float x, float y)
			{
				gl::pushMatrices();
					gl::translate(x, y);
					draw();
				gl::popMatrices();
			}

			virtual void draw()
			{					
				gl::pushMatrices();				
				gl::translate(getGlobalPosition());
					drawLayout();
				gl::popMatrices();
			}

			void setAlpha(float  alpha)
			{
				//color = Utils::colorAlpha(color, alpha);
				//bckColor = Utils::colorAlpha(bckColor, alpha);
			}	

		private:
			TextItem textItem;			
			ColorA bckColor;
			Texture tex, icon;
			float texPosX;
		};	
	}
}