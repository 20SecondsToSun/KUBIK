#pragma once

#include "IDrawable.h"
#include "elements/gamesBlock/ToolButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ToolField> ToolFieldRef;

		class ToolField: public IDispatcher
		{
		public:
			ToolField(GameId gameId):offTextColor(Color::hex(0x939eb0))
			{
				statBtn = StatToolButtonRef(new StatToolButton(gameId));
				statBtn->setColor(Color::hex(0x6798ff));				
			
				toolBtn = ConfToolButtonRef(new ConfToolButton(gameId));
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

			void swapActive()
			{
				setActive(!isActive);
			}

			void setFont(Font font)
			{
				this->font = font;
				toolBtn->setFont(font);
				statBtn->setFont(font);
			}

			void setTexts(string text1, string text2, string text3)
			{
				offText = text1;
				toolBtn->setText(text2);
				statBtn->setText(text3);
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
			Color offTextColor;
			ToolButtonRef toolBtn;
			ToolButtonRef statBtn;

			string offText;
			Font font;
		};
	}
}