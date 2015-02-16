#pragma once

#include "gui/Sprite.h"
#include "main/gamesBlock/PurchasedGamesBlock.h"
#include "main/gamesBlock/NotPurchasedGamesBlock.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class GamesBlock> GamesBlockRef;

		class GamesBlock: public Sprite
		{
		public:	
			GamesBlock(ConfigSettingsRef configSett, GameSettingsRef gameSett, Vec2f position)
				:Sprite(),
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
			
				hasGamesInShop = gameSett->getData().getNotPurchasedGames().size() != 0;

				if(hasGamesInShop)	
				{
					npGameblock  = NotPurchasedGamesBlockRef(new NotPurchasedGamesBlock(configSett, gameSett->getData().getNotPurchasedGames()));
					npGameblock->setPosition(Vec2f(0.0f, pGameblock->getHeight() + 27 + 30 + 47));
					addChild(npGameblock);
				}
			}

			virtual void activateListeners()
			{
				if(hasGamesInShop) npGameblock->activateListeners();				
				pGameblock->activateListeners();				
			}

			void draw()
			{	
				drawbg();

				drawDecorationLine1();
				Sprite::draw();
			
				if(hasGamesInShop)				
					drawDecorationLine2();	
			}	

			void drawbg()
			{
				float height = pGameblock->getHeight() + 27;
				gl::color(ci::Color::hex(0x0d0917));
				gl::pushMatrices();
					gl::translate(getGlobalPosition());					
					gl::drawSolidRect(ci::Rectf(0.0f, 0.0f, 880.0f, height));
				gl::popMatrices();
				gl::color(ci::Color::white());
			}

			void drawDecorationLine1()
			{
				gl::pushMatrices();
					gl::translate(getGlobalPosition());					
					gl::lineWidth(lineWidth);
					gl::color(lineColor);
					gl::drawLine(ci::Vec2f::zero(), ci::Vec2f(lineLength, 0.0f));				
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
					gl::drawLine(ci::Vec2f(0.0f, height), ci::Vec2f(lineLength, height));
				gl::popMatrices();
			}

			void setAlpha(float alpha)
			{
				lineColor = Utils::colorAlpha(lineColor, alpha);
				Sprite::setAlpha(alpha);	
			}

			void hide(ci::EaseFn eFunc, float time)
			{
				animatePosition = _localPosition;
				ci::Vec2f finPos = ci::Vec2f(-getWindowWidth(), _localPosition.y);
				timeline().apply( &animatePosition, finPos, time, eFunc)
					.updateFn(bind( &GamesBlock::posAnimationUpdate, this))
					.finishFn(bind( &GamesBlock::hideAnimationFinish, this));				
			}

			void hideAnimationFinish()
			{
				hideAnimCompleteSig();
			}

			void show(ci::EaseFn eFunc, float time)
			{
				ci::Vec2f finPos = ci::Vec2f(100.0f, _localPosition.y);
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

			ci::ColorA lineColor;

			bool hasGamesInShop;

			ci::Anim<ci::Vec2f> animatePosition;
		};	
	}
}