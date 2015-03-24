#pragma once

#include "gui/Sprite.h"
#include "main/gamesBlock/PurchasedGamesBlock.h"
#include "main/gamesBlock/NotPurchasedGamesBlock.h"
#include "ConfigSettings.h"
#include "GameSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class GamesBlock> GamesBlockRef;

		class GamesBlock: public Sprite
		{
		public:	
			GamesBlock(ConfigSettingsRef configSett, GameSettingsRef gameSett, const ci::Vec2f& position);
			
			virtual void activateListeners() override;
			void draw();
			void drawbg();
			void drawDecorationLine1();
			void drawDecorationLine2();
			void setAlpha(float alpha);

			void hide(const ci::EaseFn& eFunc, float time);
			void hideAnimationFinish();

			void show(const ci::EaseFn& eFunc, float time);

			void showAnimationFinish();
			void posAnimationUpdate();

			SignalVoid hideAnimCompleteSig, showAnimCompleteSig;	

		private:
			PurchasedGamesBlockRef pGameblock;
			NotPurchasedGamesBlockRef npGameblock;
			GameSettingsRef gameSett;
			ConfigSettingsRef configSett;

			bool hasGamesInShop;
			float lineWidth;
			float lineLength;
			float blockTopShiftY;

			ci::ColorA lineColor;
			ci::Anim<ci::Vec2f> animatePosition;
		};	
	}
}