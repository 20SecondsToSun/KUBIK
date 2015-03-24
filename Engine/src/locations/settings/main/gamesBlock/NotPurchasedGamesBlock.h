#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "main/gamesBlock/OneGameNotPurchased.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class NotPurchasedGamesBlock> NotPurchasedGamesBlockRef;

		class NotPurchasedGamesBlock: public Sprite
		{
		public:
			NotPurchasedGamesBlock(ConfigSettingsRef configSett, const std::vector<GamesInfo>& games);

			virtual void activateListeners() override;
			virtual void unActivateListeners() override;
			virtual void drawLayout() override;
			virtual void setPosition(ci::Vec2i position) override;

			void setAlpha(float alpha);
			void mouseUpFunction(EventGUIRef& event);

		private:	
			TextItem titleText;
		};
	}
}