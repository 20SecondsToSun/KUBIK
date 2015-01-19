#pragma once

#include "IDrawable.h"

using namespace std;
using namespace ci;

namespace kubik
{
	namespace config
	{
		class Title:public IDispatcher
		{
		public:	
			Title(Vec2i position, string name, Font font)				
				 :activityName(name),
				 activityNameFont(font),
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
			Color  activityNameColor;
			Font   activityNameFont;			
		};

		typedef std::shared_ptr<Title> TitleRef;
	}
}