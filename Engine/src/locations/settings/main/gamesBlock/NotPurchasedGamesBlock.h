#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "main/gamesBlock/OneGameNotPurchased.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class NotPurchasedGamesBlock> NotPurchasedGamesBlockRef;

		class NotPurchasedGamesBlock: public Sprite
		{
		public:
			NotPurchasedGamesBlock(ConfigSettingsRef configSett, vector<GamesInfo> games)
				:titleText(configSett->getTextItem(ConfigTextID::NOTINSTALL))
			{
				for (auto gameInfo : games)
				{
					OneGameNotPurchasedRef pGame  = OneGameNotPurchasedRef(new OneGameNotPurchased(configSett, gameInfo));						
					addChild(pGame);				
				}
			}

			virtual void activateListeners()
			{				
				for (auto game : displayList)
					game->connectEventHandler(&NotPurchasedGamesBlock::mouseUpFunction, this);
			}

			virtual void unActivateListeners()
			{
				for (auto game : displayList)
					game->disconnectEventHandler();	
			}

			void mouseUpFunction(EventGUIRef& event)
			{
				console()<<"mouseUpFunction"<<endl;
				mouseUpSignal(event);
			}

			virtual void drawLayout()
			{
				gl::color(Color::white());
				textTools().textFieldDraw(titleText, Vec2f( -8.0f,  56.0f));
			}

			void setAlpha(float alpha)
			{
				titleText.setColor(Utils::colorAlpha(titleText.getColor(), alpha));
				for (auto game : displayList)
					game->setAlpha(alpha);	
			}

			virtual void setPosition(ci::Vec2i position)		
			{	
				int i = 0;
				for (auto game : displayList)
				{
					game->setPosition(Vec2f(445.0f * (i % 2), 56.0f + 86 + 120 * (i / 2)));	
					i++;
					console()<<"not purchased game:::    "<<i<<endl;
				}

				Sprite::setPosition(position);
			}		

		private:	
			TextItem titleText;
		};
	}
}