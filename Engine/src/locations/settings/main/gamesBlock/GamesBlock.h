#pragma once

#include "IDrawable.h"
#include "Button.h"
#include "main/gamesBlock/PurchasedGamesBlock.h"
#include "main/gamesBlock/NotPurchasedGamesBlock.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class GamesBlock> GamesBlockRef;

		class GamesBlock: public IDispatcher
		{
		public:	
			GamesBlock(ConfigSettingsRef configSett, GameSettingsRef gameSett, Vec2f position)
				:lineWidth(2),
				lineLength(880.0f),
				blockTopShiftY(79.0f),
				lineColor(Color::hex(0x233442))
			{
				this->configSett = configSett;
				this->gameSett	 = gameSett;
				
				pGameblock  = PurchasedGamesBlockRef(new PurchasedGamesBlock(configSett, gameSett->getData().getPurchasedGames()));					

				hasGamesInShop = gameSett->getData().getNotPurchasedGames().size() != 0;
				if(hasGamesInShop)				
					npGameblock  = NotPurchasedGamesBlockRef(new NotPurchasedGamesBlock(configSett, gameSett->getData().getNotPurchasedGames()));

				setPosition(position);
			}

			virtual void activateListeners()
			{
				if(hasGamesInShop)
				{
					npGameblock->addMouseUpListener(&GamesBlock::mouseUpFunction, this);
					npGameblock->activateListeners();
				}

				pGameblock->addMouseUpListener(&GamesBlock::mouseUpFunction, this);	
				pGameblock->activateListeners();
			}

			virtual void unActivateListeners()
			{
				if(hasGamesInShop)
				{
					npGameblock->removeMouseUpListener();
					npGameblock->unActivateListeners();
				}

				pGameblock->removeMouseUpListener();	
				pGameblock->unActivateListeners();	
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

			void setAlpha(float  alpha)
			{
				lineColor = ColorA(lineColor.r, lineColor.g, lineColor.b, alpha);	

				if(hasGamesInShop)				
					npGameblock->setAlpha(alpha);			
			
				pGameblock->setAlpha(alpha);
			}

			virtual void setPosition(ci::Vec2i position)		
			{
				//position -= Vec2f(0, 500);
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

			ColorA lineColor;

			bool hasGamesInShop;
		};	
	}
}