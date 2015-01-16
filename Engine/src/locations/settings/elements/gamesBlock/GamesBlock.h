#pragma once

#include "IDrawable.h"
#include "Button.h"
#include "elements/gamesBlock/PurchasedGamesBlock.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class GamesBlock> GamesBlockRef;

		class GamesBlock: public IDispatcher
		{
		public:	
			GamesBlock(ConfigSettingsRef configSett, GameSettingsRef gameSett)
			{
				this->configSett = configSett;
				this->gameSett	 = gameSett;
				
				pGameblock  = PurchasedGamesBlockRef(new PurchasedGamesBlock(configSett, gameSett->getData().games));	
				pGameblock->addMouseUpListener(&GamesBlock::mouseUpFunction, this);	
			}

			virtual void draw()
			{
				gl::pushMatrices();
				gl::translate(position);
				gl::lineWidth(2);
				gl::color(Color::hex(0x233442));
				gl::drawLine(Vec2f(0.0f, 0.0f), Vec2f(880.0f, 0.0f));					
				gl::popMatrices();

				pGameblock->draw();
			}	

			virtual void setPosition(ci::Vec2i position)		
			{
				pGameblock->setPosition(position + Vec2f(0.0f, 79.0f));
				IDrawable::setPosition(position);
			}

		private:
			PurchasedGamesBlockRef pGameblock;
			GameSettingsRef gameSett;
			ConfigSettingsRef configSett;
		};	
	}
}