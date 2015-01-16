#pragma once
#include "Button.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class Checker> CheckerRef;

		class Checker: public Button
		{
		public:
			Checker(IconPair icons):Button(Rectf(0,0,0,0)),
				icons(icons),
				icon(icons.unActiveIcon),
				isActive(false),
				sdvigX(0),
				activeColor(Color::hex(0x00b6c4)),
				unActiveColor(Color::hex(0x373049)),
				color(unActiveColor)
			{
				
			}	

			virtual void draw()
			{						
				gl::color(color);
				gl::drawSolidRoundedRect(buttonArea, 55, 200);
				gl::color(Color::white());

				gl::pushMatrices();		
				gl::translate(sdvigX + buttonArea.x1, buttonArea.y1 - 3);
				gl::draw(icon);
				gl::popMatrices();				
			}

			void setActive(bool isActive)
			{
				this->isActive = isActive;
				if(isActive)
				{
					color = activeColor;
					icon = icons.activeIcon;
					sdvigX = 77.0f;
				}
				else
				{
					color = unActiveColor;
					icon = icons.unActiveIcon;
					sdvigX = 0;
				}
			}

			void swapActive()
			{
				setActive(!isActive);
			}

		private:
			float sdvigX;
			IconPair icons;
			Texture icon;
			bool isActive;
			Color color;
			Color activeColor, unActiveColor;
		};
	}
}