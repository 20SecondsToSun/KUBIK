#pragma once

#include "IDrawable.h"

using namespace std;
using namespace ci;

namespace kubik
{
	namespace config
	{
		class Title:public IDrawable
		{
		public:	
			Title():activityNameColor(Color::hex(0xffffff))
			{

			}

			virtual void draw()
			{
				gl::pushMatrices();
					gl::translate(position);
					//gl::color(ColorA(1, 1, 1, 0.5f));
					//gl::drawSolidRect(Rectf(0, 0, 350, 73));
					//gl::color(ColorA(1, 1, 1, 1.0f));
					textTools().textFieldDraw(activityName, &font, activityNameColor, Vec2f(-12, 3));				
				gl::popMatrices();
			}

			void setActivityName(string name)
			{
				activityName = name;
			}

			void setFont(Font font)
			{
				this->font = font;
			}

		private:
			string activityName;				
			Color activityNameColor;
			Font font;			
		};

		typedef std::shared_ptr<Title> TitleRef;
	}
}