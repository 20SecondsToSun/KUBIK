#pragma once
#include "gui/SimpleSpriteButton.h"
#include "ConfigSettings.h"
#include "settings/GameShowUrlEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class OneGameNotPurchased> OneGameNotPurchasedRef;

		class OneGameNotPurchased: public SimpleSpriteButton
		{
		public:
			OneGameNotPurchased(ConfigSettingsRef config, GamesInfo info)
				:nameColor(ci::Color::hex(0x939eb0)), SimpleSpriteButton(ci::Rectf(0.0f, 0.0f, 0.0f, 0.0f)),
				btnColor(ci::Color::white())
			{				
				nameText      = info.getNameText();
				nameFont	  = config->getFont("introLight36");

				miniIcon	  = info.getMiniIcon();
				gameInWebIcon = config->getTexture("gameInWebIcon");	

				textTexture = textTools().getTextField(nameText, &nameFont, nameColor);		

				event = GameShowUrlEventRef(new GameShowUrlEvent(info.getGameId()));

				setButtonArea(ci::Rectf(0, 0, 112.0f + textTexture.getWidth() + gameInWebIcon.getWidth(), 65));
			}		
			
			virtual void drawLayout()
			{				
				gl::color(btnColor);
				gl::pushMatrices();
				//gl::translate(position);
				gl::draw(miniIcon);				
				gl::draw(textTexture,   ci::Vec2f(91.0f, 2.0f));
				gl::draw(gameInWebIcon, ci::Vec2f(112.0f + textTexture.getWidth(), 13.0f));
				//gl::drawSolidRect(buttonArea);
				gl::popMatrices();		
			}

			void setAlpha(float alpha)
			{
				btnColor = ColorA(btnColor.r, btnColor.g, btnColor.b, alpha);
				nameColor = ColorA(nameColor.r, nameColor.g, nameColor.b, alpha);
			}		

		private:
			string nameText;
			Font nameFont;
			ColorA nameColor, btnColor;
			gl::Texture miniIcon, gameInWebIcon, textTexture;			
		};
	}
}