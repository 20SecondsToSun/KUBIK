#pragma once
#include "IDrawable.h"
#include "elements/gamesBlock/ToolField.h"
#include "elements/gamesBlock/Checker.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class OneGamePurchased> OneGamePurchasedRef;

		class OneGamePurchased: public IDispatcher
		{
		public:
			OneGamePurchased(ConfigSettingsRef config, GamesInfo info)
				:nameColor(Color::hex(0xffffff))
			{				
				nameText  = info.getNameText();
				nameFont  = config->getFont("introLight44");

				toolfield = ToolFieldRef(new ToolField(info.getGameId()));	
				toolfield->setFont(config->getFont("helveticaLight24"));				
				toolfield->setActive(info.isGameOn());			
				toolfield->setTexts(config->getSwitchOffText(), config->getConfigText(), config->getStatText());
				toolfield->addMouseUpListener(&OneGamePurchased::mouseUpFunction, this);				

				checker = CheckerRef(new Checker(info.getIcons()));	
				checker->addMouseUpListener(&OneGamePurchased::checkerClicked, this);
				checker->setActive(info.isGameOn());
			}

			virtual void checkerClicked( shared_ptr<kubik::Event> event )
			{
				checker->swapActive();
				toolfield->swapActive();
				
				mouseUpSignal(event);
			}
			
			virtual void draw()
			{				
				gl::color(Color::white());
				textTools().textFieldDraw(nameText, &nameFont, nameColor, Vec2f(position.x + 282.0f, position.y - 9.0f));				
				toolfield->draw();
				checker->draw();
			}

			virtual void setPosition(ci::Vec2i position)		
			{
				//checker->setPosition(position + Vec2f(, 4.0f));
				float initX = position.x + 20.0f;
				float initY = position.y + 4.0f;
				float width = 210;
				float height = 122;
				Rectf checkerRect = Rectf (initX, initY, initX + width, initY + height);
				checker->setButtonArea(checkerRect);
				toolfield->setPosition(position);
				IDrawable::setPosition(position);
			}

		private:
			string nameText;
			ci::Font nameFont;
			Color nameColor;
			ToolFieldRef toolfield;				
			CheckerRef checker;				
		};
	}
}