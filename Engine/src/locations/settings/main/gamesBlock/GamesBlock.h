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
				drawbg();

				drawDecorationLine1();
				CompositeDispatcher::draw();
			
				if(hasGamesInShop)				
					drawDecorationLine2();	
			}	

			void drawbg()
			{
				float height = pGameblock->getHeight() + 27;
				gl::color(Color::hex(0x0d0917));
				gl::pushMatrices();
					gl::translate(getGlobalPosition());					
					gl::drawSolidRect(Rectf(0.0f, 0.0f, 880.0f, height));
				gl::popMatrices();
				gl::color(Color::white());
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

			void hide(EaseFn eFunc, float time)
			{
				animatePosition = _localPosition;
				Vec2f finPos = Vec2f(-getWindowWidth(), _localPosition.y);
				timeline().apply( &animatePosition, finPos, time, eFunc)
					.updateFn(bind( &GamesBlock::posAnimationUpdate, this))
					.finishFn(bind( &GamesBlock::hideAnimationFinish, this));				
			}

			void hideAnimationFinish()
			{
				hideAnimCompleteSig();
			}

			void show(EaseFn eFunc, float time)
			{
				Vec2f finPos = Vec2f(100, _localPosition.y);
				timeline().apply( &animatePosition, finPos, time, eFunc)
					.updateFn(bind( &GamesBlock::posAnimationUpdate, this))
					.finishFn(bind( &GamesBlock::showAnimationFinish, this));				
			}

			void showAnimationFinish()
			{
				showAnimCompleteSig();
			}

			void posAnimationUpdate()
			{
				setPosition(animatePosition.value());
			}

			SignalVoid hideAnimCompleteSig, showAnimCompleteSig;
	

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

			ci::Anim<ci::Vec2f> animatePosition;
		};	
	}
}