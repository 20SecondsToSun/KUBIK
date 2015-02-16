#pragma once
#include "gui/SimpleSpriteButton.h"
#include "CheckerEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class Checker> CheckerRef;

		class Checker: public SimpleSpriteButton
		{
		public:
			Checker(ci::Rectf rect, IconPair icons, ci::Color activeColor = ci::Color::hex(0x00b6c4), ci::Color unActiveColor = ci::Color::hex(0x373049))
				:SimpleSpriteButton(rect),
				icons(icons),
				icon(icons.unActiveIcon),
				isActive(false),				
				activeColor(activeColor),
				unActiveColor(unActiveColor),
				color(unActiveColor),
				radius(55),
				startX(0),
				finishX(77.0f),
				sdvigX(0),
				iconColor(ci::Color::white())
			{
				
			}	

			virtual void mouseUp(ci::app::MouseEvent &_event)
			{
				if(inButtonField(_event.getPos()))
				{
					swapActive();
					event = CheckerEventRef(new CheckerEvent(isActive));
					Sprite::mouseUp(_event);
				}
			}

			virtual void drawLayout()
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
				color = Utils::colorAlpha(color, alpha);
				iconColor = Utils::colorAlpha(iconColor, alpha);
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
			float radius, startX, finishX;
			bool isActive;
			ci::ColorA color, iconColor;
			ci::Color activeColor, unActiveColor;
			
			IconPair icons;
			Texture icon;
		};
	}
}