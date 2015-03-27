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
			OneGamePurchased(ConfigSettingsRef config, const GamesInfo& info);

			virtual void drawLayout();
			virtual void activateListeners();
			virtual void unActivateListeners();

			void setAlpha(float  alpha);
			void checkerClicked(EventGUIRef& event);
			void freeze();
			void unFreeze();
			GameId getGameID();

		private:
			std::string nameText;
			ci::Font nameFont;
			ci::ColorA nameColor;
			ci::Vec2f namePosition;
			ToolFieldRef toolfield;				
			CheckerRef checker;	
			ci::Rectf checkerArea;
			GamesInfo info;
		};
	}
}