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
			Checker(IconPair icons, Color activeColor = Color::hex(0x00b6c4), Color unActiveColor = Color::hex(0x373049)):Button(Rectf(0,0,0,0)),
				icons(icons),
				icon(icons.unActiveIcon),
				isActive(false),				
				activeColor(activeColor),
				unActiveColor(unActiveColor),
				color(unActiveColor),
				radius(55),
				startX(0),
				finishX(77.0f),
				sdvigX(startX)
			{
				
			}	

			virtual void draw()
			{						
				gl::color(color);
				gl::drawSolidRoundedRect(buttonArea, radius, 200);
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
					sdvigX = finishX;
				}
				else
				{
					color = unActiveColor;
					icon = icons.unActiveIcon;
					sdvigX = startX;
				}
			}


			void setRadius(float radius)
			{
				this->radius = radius;
			}

			void setStartX(float x)
			{
				this->startX = x;
			}

			void setFinishX(float x)
			{
				this->finishX = x;
			}

			void swapActive()
			{
				setActive(!isActive);
			}

		protected:
			float sdvigX;
			IconPair icons;
			Texture icon;
			bool isActive;
			Color color;
			Color activeColor, unActiveColor;
			float radius, startX, finishX;
		};
	}
}