#pragma once

#include "IDispatcher.h"

using namespace std;
using namespace ci;

namespace kubik
{
	namespace config
	{
		class Title:public IDispatcher
		{
		public:	
			Title(ConfigSettingsRef configSettings, Vec2i position)				
				 :activityName("Promo activity"),
				 activityNameFont(configSettings->getFont("introLight44")),
				 activityNameColor(Color::white())
			{
				IDrawable::setPosition(position);
			}

			virtual void draw()
			{
				gl::pushMatrices();
					gl::translate(position);	
					textTools().textFieldDraw(activityName, &activityNameFont, activityNameColor, Vec2f(-12, 3));				
				gl::popMatrices();
			}

			void setAlpha(float  alpha)
			{
				activityNameColor = ColorA(activityNameColor.r, activityNameColor.g, activityNameColor.b, alpha);	
			}

			void setActivityName(string name)
			{
				activityName = name;
			}

			void setFont(Font font)
			{
				activityNameFont = font;
			}

		private:
			string activityName;				
			ColorA  activityNameColor;
			Font   activityNameFont;			
		};

		typedef std::shared_ptr<Title> TitleRef;
	}
}