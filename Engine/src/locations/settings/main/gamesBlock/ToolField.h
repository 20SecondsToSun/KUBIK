#pragma once

#include "gui/Sprite.h"

#include "main/gamesBlock/ToolButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ToolField> ToolFieldRef;

		class ToolField: public Sprite
		{
		public:
			ToolField(ConfigSettingsRef config, GamesInfo info)
				:Sprite(),			
				offText(config->getTextItem(ConfigTextID::SWITCH_OFF))
			{	
				Rectf ba1 = Rectf(295.0f, 65.0f, 295.0f + 180, 65.0f + 55);				
				Rectf ba2 = Rectf(295.0f + 205, 65.0f, 295.0f + 205 + 180 + 10, 65.0f + 55);

				statBtn = StatToolButtonRef(new StatToolButton(ba1, info.getGameId(), config->getTextItem(ConfigTextID::STAT)));
				statBtn->setColor(Color::hex(0x6798ff));
				addChild(statBtn);

				toolBtn = ConfToolButtonRef(new ConfToolButton(ba2, info.getGameId(), config->getTextItem(ConfigTextID::CONFIG)));
				toolBtn->setColor(Color::hex(0x00f067));
				addChild(toolBtn);
				setActive(info.isGameOn());			
			}	

			virtual void draw()
			{	
				gl::pushMatrices();				
				gl::translate(getGlobalPosition());
				if(!isActive)
					drawOffMessage();
				gl::popMatrices();

				if(isActive)
					Sprite::draw();
			}

			void drawOffMessage()
			{	
				gl::color(Color::white());
				textTools().textFieldDraw(offText, Vec2f(289, 55));			
			}		

			void setActive(bool isActive)
			{
				this->isActive = isActive;

				if(isActive)
				{
					toolBtn->connectEventHandler(&ToolField::mouseUpFunction, this);
					statBtn->connectEventHandler(&ToolField::mouseUpFunction, this);
				}
				else
				{
					toolBtn->disconnectEventHandler();
					statBtn->disconnectEventHandler();
				}
			}

			virtual void unActivateListeners()
			{
				toolBtn->disconnectEventHandler();
				statBtn->disconnectEventHandler();
			}

			void mouseUpFunction(EventGUIRef& event)
			{
				mouseUpSignal(event);
			}

			virtual void activateListeners()
			{
				setActive(isActive);
			}		

			void setAlpha(float  alpha)
			{		
				offText.setColor(Utils::colorAlpha(offText.getColor(), alpha));
				Sprite::setAlpha(alpha);
			}

			void swapActive()
			{
				setActive(!isActive);
			}

		private:
			bool isActive;
		
			ConfToolButtonRef toolBtn;
			StatToolButtonRef statBtn;

			TextItem offText;			
		};
	}
}