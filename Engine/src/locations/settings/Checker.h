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
				activeBordercolor(ci::Color::hex(0xbc6d10)),
				unActiveBordercolor(ci::Color::hex(0xa05b09)),
				icons(icons),
				icon(icons.unActiveIcon),
				isActive(false),				
				activeColor(activeColor),
				unActiveColor(unActiveColor),
				color(unActiveColor),
				radius(55.0f),
				startX(0.0f),
				finishX(77.0f),
				sdvigX(0.0f),
				iconColor(ci::Color::white())
			{
				event = CheckerEventRef(new CheckerEvent(isActive));
			}	

			virtual void mouseUp(ci::app::MouseEvent &_event)
			{
				if(inButtonField(_event.getPos()))
				{
					swapActive();
					CheckerEventRef eventref = static_pointer_cast<CheckerEvent>(event);
					eventref->setActive(isActive);	
					Sprite::mouseUp(_event);
				}
			}

			virtual void drawLayout()
			{
				gl::color(color);
				gl::drawSolidRoundedRect(buttonArea, radius, 200.0f);
				gl::color(Color::white());
				gl::color(borderColor);
				gl::drawStrokedRoundedRect(buttonArea, radius, 200.0f);
				gl::color(Color::white());
				gl::pushMatrices();		
				gl::translate(sdvigX + buttonArea.x1, buttonArea.y1 - 3.0f);
				gl::color(iconColor);
				gl::draw(icon);
				gl::popMatrices();							
			}

			void setAlpha(float  alpha)
			{
				color	  = Utils::colorAlpha(color, alpha);
				iconColor = Utils::colorAlpha(iconColor, alpha);
			}

			void setActive(bool isActive)
			{
				this->isActive = isActive;

				if(isActive)
				{
					color  = activeColor;
					borderColor = activeBordercolor;
					icon   = icons.activeIcon;
					sdvigX = finishX;
				}
				else
				{
					color  = unActiveColor;
					borderColor = unActiveBordercolor;
					icon   = icons.unActiveIcon;
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
				//console()<<"active::::::::::::::::  "<<isActive<<endl;
				setActive(!isActive);
			}

			void setBorderColorActive(ci::ColorA color)
			{
				activeBordercolor = color;
				setActive(isActive);
			}

			void setBorderColorUnActive(ci::ColorA color)
			{
				unActiveBordercolor = color;
				setActive(isActive);
			}	

		protected:
			float sdvigX;
			float radius, startX, finishX;
			bool isActive;
			ci::ColorA color, borderColor, iconColor, activeBordercolor, unActiveBordercolor;
			ci::Color activeColor, unActiveColor;
			
			IconPair icons;
			ci::gl::Texture icon;
		};
	}
}