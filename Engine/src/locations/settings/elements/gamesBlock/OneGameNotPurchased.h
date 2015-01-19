#pragma once
#include "IDispatcher.h"
#include "ConfigSettings.h"
#include "settings/GameShowUrlEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class OneGameNotPurchased> OneGameNotPurchasedRef;

		class OneGameNotPurchased: public Button
		{
		public:
			OneGameNotPurchased(ConfigSettingsRef config, GamesInfo info)
				:nameColor(Color::hex(0x939eb0)), Button(Rectf(0,0,0,0))
			{				
				nameText  = info.getNameText();
				nameFont  = config->getFont("introLight36");

				miniIcon = info.getMiniIcon();
				gameInWebIcon = config->getTexture("gameInWebIcon");	

				textTexture = textTools().getTextField(nameText, &nameFont, nameColor);		

				event = GameShowUrlEventRef(new GameShowUrlEvent(info.getGameId()));
			}		
			
			virtual void draw()
			{				
				gl::color(Color::white());
				gl::pushMatrices();
				gl::translate(position);
				gl::draw(miniIcon);				
				gl::draw(textTexture, Vec2f(91.0f, 2.0f));
				gl::draw(gameInWebIcon, Vec2f(112.0f + textTexture.getWidth(), 13.0f));
				gl:popMatrices();		
			}

			virtual void setPosition(ci::Vec2i position)		
			{	
				float initX = position.x;
				float initY = position.y;
				float width = 112.0f + textTexture.getWidth() + gameInWebIcon.getWidth();
				setButtonArea(Rectf(initX, initY, initX + width, initY +miniIcon.getHeight()));
				IDrawable::setPosition(position);
			}

		private:
			string nameText;
			Font nameFont;
			Color nameColor;
			gl::Texture miniIcon, gameInWebIcon, textTexture;			
		};
	}
}