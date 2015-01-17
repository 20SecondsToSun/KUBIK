#pragma once
#include "IDispatcher.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "elements/gamesBlock/OneGameNotPurchased.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class NotPurchasedGamesBlock> NotPurchasedGamesBlockRef;

		class NotPurchasedGamesBlock: public IDispatcher
		{
		public:
			NotPurchasedGamesBlock(ConfigSettingsRef configSett, vector<GamesInfo> games)
				:titleColor(Color::hex(0x939eb0))
			{

				titleText = configSett->getNotInstallGamesText();
				titleFont = configSett->getFont("introLight36");

				for (auto gameInfo : games)
				{
					OneGameNotPurchasedRef pGame  = OneGameNotPurchasedRef(new OneGameNotPurchased(configSett, gameInfo));	
					pGame->addMouseUpListener(&OneGameNotPurchased::mouseUpFunction, this);	
					addChild(pGame);				
				}
			}

			virtual void draw()
			{
				gl::color(Color::white());
				textTools().textFieldDraw(titleText, &titleFont, titleColor, Vec2f(position.x - 8.0f, position.y + 56.0f));
				IDispatcher::draw();
			}

			virtual void setPosition(ci::Vec2i position)		
			{	
				int i = 0;
				for (auto game : displayList)
				{
					game->setPosition(position + Vec2f(445.0f * (i % 2), 56.0f + 86 + 120 * (i / 2)));
					game->addMouseUpListener(&OneGameNotPurchased::mouseUpFunction, this);
					i++;
				}
				IDrawable::setPosition(position);
			}		

		private:	
			string titleText;
			Color titleColor;
			Font titleFont;
		};
	}
}