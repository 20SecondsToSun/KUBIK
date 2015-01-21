#pragma once
#include "IDrawable.h"
#include "main/gamesBlock/OneGamePurchased.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PurchasedGamesBlock> PurchasedGamesBlockRef;

		class PurchasedGamesBlock: public IDispatcher
		{
		public:
			PurchasedGamesBlock(ConfigSettingsRef configSett, vector<GamesInfo> games)
				:oneGamePurchasedHeight(186)
			{
				purchasedGamesSize = games.size();

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
					game->setPosition(position + Vec2f(0.0f, oneGamePurchasedHeight * i++));					
				
				IDrawable::setPosition(position);
			}

			void setAlpha(float alpha)
			{
				for (auto game : displayList)
					game->setAlpha(alpha);
			}

			virtual void activateListeners()
			{
				for (auto game : displayList)
				{
					game->addMouseUpListener(&PurchasedGamesBlock::mouseUpFunction, this);
					game->activateListeners();
				}
			}

			virtual void unActivateListeners()
			{
				for (auto game : displayList)
				{
					game->removeMouseUpListener();
					game->unActivateListeners();
				}
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