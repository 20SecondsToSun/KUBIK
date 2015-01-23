#pragma once

#include "gui/CompositeDispatcher.h"

#include "main/gamesBlock/ToolField.h"
#include "Checker.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class OneGamePurchased> OneGamePurchasedRef;

		class OneGamePurchased: public CompositeDispatcher
		{
		public:
			OneGamePurchased(ConfigSettingsRef config, GamesInfo info)
				:nameColor(Color::hex(0xffffff)),
				 nameText(info.getNameText()),
				 nameFont(config->getFont("introLight44")),
				 checkerArea(Rectf (20.0f, 4.0f, 214.0f, 126.0f)),
				 namePosition(Vec2f(282.0f, -10.0f))
			{			
				toolfield = ToolFieldRef(new ToolField(config, info.getGameId()));	
				toolfield->setActive(info.isGameOn());
				addChild(toolfield);

				checker = CheckerRef(new Checker(checkerArea, info.getIcons()));					
				checker->setActive(info.isGameOn());
				addChild(checker);
			}

			virtual void checkerClicked(EventGUIRef event)
			{
				checker->swapActive();
				toolfield->swapActive();				
				mouseUpSignal(event);
			}

			virtual void drawLayout()
			{				
				textTools().textFieldDraw(nameText, &nameFont, nameColor, namePosition);
				gl::color(Color::white());
				CompositeDispatcher::drawLayout();
			}

			void setAlpha(float  alpha)
			{
				nameColor = Utils::colorAlpha(nameColor, alpha);
				CompositeDispatcher::setAlpha(alpha);
			}

			virtual void activateListeners()
			{
				toolfield->addMouseUpListener(&OneGamePurchased::mouseUpFunction, this);
				checker->addMouseUpListener(&OneGamePurchased::checkerClicked, this);
				CompositeDispatcher::activateListeners();
			}

			virtual void unActivateListeners()
			{
				toolfield->removeMouseUpListener();
				checker->removeMouseUpListener();
			}

		private:
			string nameText;
			Font nameFont;
			ColorA nameColor;
			Vec2f namePosition;
			ToolFieldRef toolfield;				
			CheckerRef checker;	
			Rectf checkerArea;
		};
	}
}