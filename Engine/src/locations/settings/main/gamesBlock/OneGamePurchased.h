#pragma once
#include "IDrawable.h"
#include "main/gamesBlock/ToolField.h"
#include "Checker.h"

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

				toolfield = ToolFieldRef(new ToolField(config, info.getGameId()));	
				toolfield->setActive(info.isGameOn());	

				checker = CheckerRef(new Checker(info.getIcons()));					
				checker->setActive(info.isGameOn());
			}

			virtual void checkerClicked(shared_ptr<kubik::Event> event)
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

			void setAlpha(float  alpha)
			{
				nameColor = ColorA(nameColor.r, nameColor.g, nameColor.b, alpha);
				toolfield->setAlpha(alpha);
				checker->setAlpha(alpha);
			}

			virtual void setPosition(ci::Vec2i position)		
			{
				float initX = position.x + 20.0f;
				float initY = position.y + 4.0f;
				float width = 210;
				float height = 122;
				Rectf checkerRect = Rectf (initX, initY, initX + width, initY + height);
				checker->setButtonArea(checkerRect);
				toolfield->setPosition(position);
				IDrawable::setPosition(position);
			}

			virtual void activateListeners()
			{
				toolfield->addMouseUpListener(&OneGamePurchased::mouseUpFunction, this);
				checker->addMouseUpListener(&OneGamePurchased::checkerClicked, this);
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
			ToolFieldRef toolfield;				
			CheckerRef checker;				
		};
	}
}