#pragma once

#include "gui/CompositeDispatcher.h"
#include "main/gamesBlock/PurchasedGamesBlock.h"
#include "main/gamesBlock/NotPurchasedGamesBlock.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class GamesBlock> GamesBlockRef;

		class GamesBlock: public CompositeDispatcher
		{
		public:	
			GamesBlock(ConfigSettingsRef configSett, GameSettingsRef gameSett, Vec2f position)
				:CompositeDispatcher(),
				lineWidth(2),
				lineLength(880.0f),
				blockTopShiftY(79.0f),
				lineColor(Color::hex(0x233442)),
				configSett(configSett),
				gameSett(gameSett)
			{
				setPosition(position);	

				pGameblock  = PurchasedGamesBlockRef(new PurchasedGamesBlock(configSett, gameSett->getData().getPurchasedGames()));		
				pGameblock->setPosition(Vec2f(0.0f, blockTopShiftY));
				addChild(pGameblock);
				//console()<<"get game position:::::::  "<<pGameblock->getGlobalPosition()<<endl;
				/*hasGamesInShop = gameSett->getData().getNotPurchasedGames().size() != 0;
				if(hasGamesInShop)				
					npGameblock  = NotPurchasedGamesBlockRef(new NotPurchasedGamesBlock(configSett, gameSett->getData().getNotPurchasedGames()));	*/			
			}

			virtual void activateListeners()
			{
				/*if(hasGamesInShop)
				{
					npGameblock->addMouseUpListener(&GamesBlock::mouseUpFunction, this);
					npGameblock->activateListeners();
				}*/

				pGameblock->addMouseUpListener(&GamesBlock::mouseUpFunction, this);	
				CompositeDispatcher::activateListeners();
			}

		/*	void drawLayout()
			{
				drawDecorationLine1();

				if(hasGamesInShop)				
					drawDecorationLine2();	

				CompositeDispatcher::drawLayout();
			}*/

			void draw()
			{	
				drawDecorationLine1();
				CompositeDispatcher::draw();
			
				if(hasGamesInShop)				
					drawDecorationLine2();	
			}	

			void drawDecorationLine1()
			{
				gl::pushMatrices();
					gl::translate(getGlobalPosition());					
					gl::lineWidth(lineWidth);
					gl::color(lineColor);
					gl::drawLine(Vec2f::zero(), Vec2f(lineLength, 0.0f));				
				gl::popMatrices();
			}

			void drawDecorationLine2()
			{
				float height = pGameblock->getHeight() + 27;
				gl::pushMatrices();
					gl::translate(getGlobalPosition());
					gl::lineWidth(lineWidth);
					gl::color(lineColor);
					gl::translate(0.0f, blockTopShiftY);
					gl::drawLine(Vec2f(0.0f, height), Vec2f(lineLength, height));
				gl::popMatrices();
			}

			void setAlpha(float  alpha)
			{
				lineColor = Utils::colorAlpha(lineColor, alpha);
				CompositeDispatcher::setAlpha(alpha);	
			}

			//virtual void setPosition(ci::Vec2i position)		
			//{				
				/*if(hasGamesInShop)
				{
					float height = pGameblock->getHeight() + 27;
					npGameblock->setPosition(position + Vec2f(0.0f, blockTopShiftY + height));
				}*/
				//IDrawable::setPosition(position);
			//}

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