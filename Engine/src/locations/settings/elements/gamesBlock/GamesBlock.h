#pragma once

#include "IDrawable.h"
#include "Button.h"
#include "elements/gamesBlock/PurchasedGamesBlock.h"
#include "elements/gamesBlock/NotPurchasedGamesBlock.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class GamesBlock> GamesBlockRef;

		class GamesBlock: public IDispatcher
		{
		public:	
			GamesBlock(ConfigSettingsRef configSett, GameSettingsRef gameSett)
				:lineWidth(2),
				lineLength(880.0f),
				blockTopShiftY(79.0f),
				lineColor(Color::hex(0x233442))
			{
				this->configSett = configSett;
				this->gameSett	 = gameSett;
				
				pGameblock  = PurchasedGamesBlockRef(new PurchasedGamesBlock(configSett, gameSett->getData().getPurchasedGames()));	
				pGameblock->addMouseUpListener(&GamesBlock::mouseUpFunction, this);	

				hasGamesInShop = gameSett->getData().getNotPurchasedGames().size() != 0;

				if(hasGamesInShop)
				{
					npGameblock  = NotPurchasedGamesBlockRef(new NotPurchasedGamesBlock(configSett, gameSett->getData().getNotPurchasedGames()));	
					npGameblock->addMouseUpListener(&GamesBlock::mouseUpFunction, this);	
				}							
			}

			virtual void draw()
			{				
				drawDecorationLine1();
				pGameblock->draw();

				if(hasGamesInShop)
				{
					drawDecorationLine2();
					npGameblock->draw();
				}
			}	

			void drawDecorationLine1()
			{
				gl::pushMatrices();
					gl::translate(position);
					gl::lineWidth(lineWidth);
					gl::color(lineColor);
					gl::drawLine(Vec2f(0.0f, 0.0f), Vec2f(lineLength, 0.0f));				
				gl::popMatrices();
			}

			void drawDecorationLine2()
			{
				float height = pGameblock->getHeight() + 27;
				gl::pushMatrices();
					gl::translate(position);
					gl::lineWidth(lineWidth);
					gl::color(lineColor);
					gl::translate(0.0f, blockTopShiftY);
					gl::drawLine(Vec2f(0.0f, height), Vec2f(lineLength, height));
				gl::popMatrices();
			}

			virtual void setPosition(ci::Vec2i position)		
			{
				position -= Vec2f(0, 500);
				pGameblock->setPosition(position + Vec2f(0.0f, blockTopShiftY));
				if(hasGamesInShop)
				{
					float height = pGameblock->getHeight() + 27;
					npGameblock->setPosition(position + Vec2f(0.0f, blockTopShiftY + height));
				}
				IDrawable::setPosition(position);
			}

		private:
			PurchasedGamesBlockRef pGameblock;
			NotPurchasedGamesBlockRef npGameblock;
			GameSettingsRef gameSett;
			ConfigSettingsRef configSett;

			float lineWidth;
			float lineLength;
			float blockTopShiftY;

			Color lineColor;

			bool hasGamesInShop;
		};	
	}
}