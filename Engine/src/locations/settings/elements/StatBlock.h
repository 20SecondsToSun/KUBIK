#pragma once

#include "IDrawable.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace kubik
{
	namespace config
	{
		class StatBlock:public IDrawable
		{
		public:

			StatBlock():title1("Сыграли"), 
						title2("Напечатали фото"),
						title3("Соц.сети и email"),
						titleColor(Color::hex(0x939eb0)),
						numsColor(Color::hex(0xffffff))
			{

			}

			virtual void draw()
			{
				//gl::color(ColorA(1, 1, 1, 0.5f));
				gl::pushMatrices();
					gl::translate(position);
					//gl::drawSolidRect(Rectf(0, 0, 880, 190));
					//gl::color(ColorA(1, 1, 1, 1.0f));
					textTools().textFieldDraw(title1, &titleFont, titleColor, Vec2f(-4, -4));
					textTools().textFieldDraw(title2, &titleFont, titleColor, Vec2f(-4 + 350, -4));
					textTools().textFieldDraw(title3, &titleFont, titleColor, Vec2f(-4 + 645, -4));	
					textTools().textFieldDraw(to_string(playedTimes), &numsFont, numsColor, Vec2f(-25, -4));	
					textTools().textFieldDraw(to_string(printedPhotos), &numsFont, numsColor, Vec2f(325, -4));	
					textTools().textFieldDraw(to_string(sharedAndEmail), &numsFont, numsColor, Vec2f(620, -4));	

					gl::lineWidth(2);
					gl::color(Color::hex(0x233442));
					gl::drawLine(Vec2f(0, 190), Vec2f(880, 190));
					gl::drawLine(Vec2f(880/3.0f, 0), Vec2f(880/3.0f, 190));
					gl::drawLine(Vec2f(2*880/3.0f, 0), Vec2f(2*880/3.0f, 190));
					gl::color(Color::white());
				gl::popMatrices();				
			}		

			void setTitleFont(Font font)
			{
				titleFont = font;
			}

			void setNumsFont(Font font)
			{
				numsFont = font;
			}

			void setPlayedTimes(int num)
			{
				playedTimes = num;
			}

			void setPrintedPhotos(int num)
			{
				printedPhotos = num;
			}

			void setSharedAndEmail(int num)
			{
				sharedAndEmail = num;
			}

		private:			
			Font numsFont;
			Font titleFont;

			string title1, title2, title3;
			Color titleColor, numsColor;

			int playedTimes;
			int printedPhotos;
			int sharedAndEmail;
		};

		typedef std::shared_ptr<StatBlock> StatBlockRef;
	}
}