#pragma once

#include "IDrawable.h"
#include "main/gamesBlock/ToolButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ToolField> ToolFieldRef;

		class ToolField: public IDispatcher
		{
		public:
			ToolField(ConfigSettingsRef config, GameId gameId)
				:offTextColor(Color::hex(0x939eb0)),
				offText(config->getSwitchOffText())
			{			
				font =	config->getFont("helveticaLight24");
				
				statBtn = StatToolButtonRef(new StatToolButton(gameId, config->getStatText(), font));
				statBtn->setColor(Color::hex(0x6798ff));	
				
				toolBtn = ConfToolButtonRef(new ConfToolButton(gameId, config->getConfigText(), font));
				toolBtn->setColor(Color::hex(0x00f067));		
				setActive(false);				
			}			

			virtual void draw()
			{
				(isActive) ? drawControls() : drawOffMessage();
			}

			void drawOffMessage()
			{				
				textTools().textFieldDraw(offText, &font, offTextColor, Vec2f(position.x + 289, position.y + 55));
			}

			void drawControls() const
			{				
				toolBtn->draw();
				statBtn->draw();
			}

			void setActive(bool isActive)
			{
				this->isActive = isActive;

				if(isActive)
				{
					toolBtn->addMouseUpListener(&ToolField::mouseUpFunction, this);
					statBtn->addMouseUpListener(&ToolField::mouseUpFunction, this);
				}
				else
				{
					toolBtn->removeMouseUpListener();
					statBtn->removeMouseUpListener();
				}
			}

			void setAlpha(float  alpha)
			{
				offTextColor = ColorA(offTextColor.r, offTextColor.g, offTextColor.b, alpha);
				toolBtn->setAlpha(alpha);
				statBtn->setAlpha(alpha);
			}

			void swapActive()
			{
				setActive(!isActive);
			}		

			virtual void setPosition(ci::Vec2i position)		
			{
				IDrawable::setPosition(position);
				float shiftX = 295.0f + position.x;
				float shiftY = 65.0f  + position.y;	
				toolBtn->setButtonArea(Rectf(shiftX, shiftY, shiftX + 180, shiftY + 55));

				shiftX += 205;
				statBtn->setButtonArea(Rectf(shiftX, shiftY, shiftX + 180 + 10, shiftY + 55));
			}

		private:
			bool isActive;
			ColorA offTextColor;
			ConfToolButtonRef toolBtn;
			StatToolButtonRef statBtn;

			string offText;
			Font font;
		};
	}
}