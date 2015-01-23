#pragma once

#include "gui/CompositeDispatcher.h"
#include "main/gamesBlock/OneGamePurchased.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PurchasedGamesBlock> PurchasedGamesBlockRef;

		class PurchasedGamesBlock: public CompositeDispatcher
		{
		public:
			PurchasedGamesBlock(ConfigSettingsRef configSett, vector<GamesInfo> games)
				:oneGamePurchasedHeight(186),
				purchasedGamesSize(games.size())
			{	
				int i = 0;
				for (auto gameInfo : games)
				{
					OneGamePurchasedRef game = OneGamePurchasedRef(new OneGamePurchased(configSett, gameInfo));	
					game->setPosition(Vec2f(0.0f, oneGamePurchasedHeight * i++));
					addChild(game);				
				}
			}

			virtual void activateListeners()
			{
				for (auto game : displayList)				
					game->addMouseUpListener(&PurchasedGamesBlock::mouseUpFunction, this);
		
				CompositeDispatcher::activateListeners();
			}

			float getHeight() const
			{
				return oneGamePurchasedHeight * purchasedGamesSize;
			}

		private:
			int oneGamePurchasedHeight;
			int purchasedGamesSize;
		};
	}
}