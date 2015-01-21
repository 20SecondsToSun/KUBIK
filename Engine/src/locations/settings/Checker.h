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
				sdvigX(startX),
				iconColor(Color::white())
			{
				
			}	

			virtual void draw()
			{						
				gl::color(color);
				gl::drawSolidRoundedRect(buttonArea, radius, 200);
				gl::color(Color::white());

				gl::pushMatrices();		
				gl::translate(sdvigX + buttonArea.x1, buttonArea.y1 - 3);
				gl::color(iconColor);
				gl::draw(icon);
				gl::popMatrices();				
			}

			void setAlpha(float  alpha)
			{
				color = ColorA(color.r, color.g, color.b, alpha);
				iconColor = ColorA(iconColor.r, iconColor.g, iconColor.b, alpha);
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

			bool getValue()
			{
				return isActive;
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
			ColorA color, iconColor;
			Color activeColor, unActiveColor;
			float radius, startX, finishX;
		};
	}
}