#pragma once

#include "gui/Sprite.h"
#include "main/gamesBlock/OneGamePurchased.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PurchasedGamesBlock> PurchasedGamesBlockRef;

		class PurchasedGamesBlock: public Sprite
		{
		public:
			PurchasedGamesBlock(ConfigSettingsRef configSett, const std::vector<GamesInfo>& games);
			
			virtual void activateListeners() override;
			virtual void unActivateListeners() override;
			void freezeChecker(const GameId& id);
			void unFreezeChecker();
			float getHeight() const;

		private:
			int oneGamePurchasedHeight;
			int purchasedGamesSize;
			std::vector<OneGamePurchasedRef> mainGamesVec;
		};
	}
}