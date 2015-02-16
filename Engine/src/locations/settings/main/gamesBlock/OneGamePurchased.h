#pragma once

#include "gui/Sprite.h"
#include "main/gamesBlock/ToolField.h"
#include "GameChecker.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class OneGamePurchased> OneGamePurchasedRef;

		class OneGamePurchased: public Sprite
		{
		public:
			OneGamePurchased(ConfigSettingsRef config, GamesInfo info)
				:nameColor(Color::hex(0xffffff)),
				 nameText(info.getNameText()),
				 nameFont(config->getFont("introLight44")),
				 checkerArea(Rectf (20.0f, 4.0f, 214.0f, 126.0f)),
				 namePosition(Vec2f(282.0f, -10.0f))
			{			
				toolfield = ToolFieldRef(new ToolField(config, info));					
				addChild(toolfield);

				checker = GameCheckerRef(new GameChecker(checkerArea, info));		
				addChild(checker);
			}			

			virtual void drawLayout()
			{
				gl::color(Color::white());
				textTools().textFieldDraw(nameText, &nameFont, nameColor, namePosition);				
				Sprite::drawLayout();
			}

			void setAlpha(float  alpha)
			{
				nameColor = Utils::colorAlpha(nameColor, alpha);
				Sprite::setAlpha(alpha);
			}

			virtual void activateListeners()
			{
				checker->connectEventHandler(&OneGamePurchased::checkerClicked, this);
			}

			virtual void unActivateListeners()
			{
				toolfield->unActivateListeners();
				checker->disconnectEventHandler();
			}

			void checkerClicked(EventGUIRef& event)
			{
				toolfield->swapActive();	
				mouseUpSignal(event);
			}

		private:
			string nameText;
			Font nameFont;
			ColorA nameColor;
			Vec2f namePosition;
			ToolFieldRef toolfield;				
			CheckerRef checker;	
			Rectf checkerArea;
		};
	}
}