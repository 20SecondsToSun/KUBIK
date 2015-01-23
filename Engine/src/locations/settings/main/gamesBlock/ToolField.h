#pragma once

#include "gui/CompositeDispatcher.h"

#include "main/gamesBlock/ToolButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ToolField> ToolFieldRef;

		class ToolField: public CompositeDispatcher
		{
		public:
			ToolField(ConfigSettingsRef config, GameId gameId)
				:CompositeDispatcher(),
				offTextColor(Color::hex(0x939eb0)),
				offText(config->getSwitchOffText()),
				font(config->getFont("helveticaLight24"))
			{					
				Rectf ba1 = Rectf(295.0f, 65.0f, 295.0f + 180, 65.0f + 55);				
				Rectf ba2 = Rectf(295.0f + 205, 65.0f, 295.0f + 205 + 180 + 10, 65.0f + 55);

				statBtn = StatToolButtonRef(new StatToolButton(ba1, gameId, config->getStatText(), font));
				statBtn->setColor(Color::hex(0x6798ff));
				addChild(statBtn);

				toolBtn = ConfToolButtonRef(new ConfToolButton(ba2, gameId, config->getConfigText(), font));
				toolBtn->setColor(Color::hex(0x00f067));
				addChild(toolBtn);
				setActive(false);				
			}	

			virtual void draw()
			{	
				gl::pushMatrices();				
				gl::translate(getGlobalPosition());
				if(!isActive)
					drawOffMessage();
				gl::popMatrices();

				if(isActive)
					CompositeDispatcher::draw();
			}

			void drawOffMessage()
			{	
				gl::color(Color::white());
				textTools().textFieldDraw(offText, &font, offTextColor, Vec2f(289, 55));			
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

			virtual void activateListeners()
			{
				setActive(isActive);
				CompositeDispatcher::activateListeners();
			}		

			void setAlpha(float  alpha)
			{		
				offTextColor = Utils::colorAlpha(offTextColor, alpha);
				CompositeDispatcher::setAlpha(alpha);
			}

			void swapActive()
			{
				setActive(!isActive);
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