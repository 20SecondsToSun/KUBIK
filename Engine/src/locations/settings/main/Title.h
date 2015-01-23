#pragma once
#include "gui/Dispatcher.h"

using namespace std;
using namespace ci;

namespace kubik
{
	namespace config
	{
		class Title:public Dispatcher
		{
		public:	
			Title(ConfigSettingsRef configSettings, Vec2i position)				
				 :Dispatcher(),
				 name("Promo activity"),
				 font(configSettings->getFont("introLight44")),
				 color(Color::white())
			{
				setPosition(position);
			}

			virtual void drawLayout()
			{
				textTools().textFieldDraw(name, &font, color, Vec2f(-12, 3));				
			}

			void setAlpha(float alpha)
			{
				color = Utils::colorAlpha(color, alpha);	
			}

			void setActivityName(string name)
			{
				this->name = name;
			}

		private:
			string name;				
			ColorA  color;	
			Font font;
		};

		typedef std::shared_ptr<Title> TitleRef;
	}
}