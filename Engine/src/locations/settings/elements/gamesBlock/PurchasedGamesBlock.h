#pragma once
#include "IDrawable.h"
#include "elements/gamesBlock/OneGamePurchased.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PurchasedGamesBlock> PurchasedGamesBlockRef;

		class PurchasedGamesBlock: public IDispatcher
		{
		public:
			PurchasedGamesBlock(ConfigSettingsRef configSett, vector<GamesInfo> games)
			{				
				for (auto gameInfo : games)
				{
					OneGamePurchasedRef pGame  = OneGamePurchasedRef(new OneGamePurchased(configSett, gameInfo));	
					addChild(pGame);				
				}
			}

			virtual void setPosition(ci::Vec2i position)		
			{
				int i = 0;
				for (auto game : displayList)
				{
					game->setPosition(position + Vec2f(0.0f, (184 + 2) * i++));
					game->addMouseUpListener(&PurchasedGamesBlock::mouseUpFunction, this);
				}
				IDrawable::setPosition(position);
			}	
		};
	}
}