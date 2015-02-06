#pragma once
#include "IDispatcher.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "main/gamesBlock/OneGameNotPurchased.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class NotPurchasedGamesBlock> NotPurchasedGamesBlockRef;

		class NotPurchasedGamesBlock: public IDispatcher
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
					game->addMouseUpListener(&OneGameNotPurchased::mouseUpFunction, this);	
			}

			virtual void unActivateListeners()
			{
				for (auto game : displayList)
					game->removeMouseUpListener();	
			}

			virtual void draw()
			{
				gl::color(Color::white());
				textTools().textFieldDraw(titleText, Vec2f(position.x - 8.0f, position.y + 56.0f));
				IDispatcher::draw();
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
					game->setPosition(position + Vec2f(445.0f * (i % 2), 56.0f + 86 + 120 * (i / 2)));
					i++;
				}
				IDrawable::setPosition(position);
			}		

		private:	
			TextItem titleText;
		};
	}
}