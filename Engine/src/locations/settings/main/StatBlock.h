#pragma once
#include "gui/Dispatcher.h"
#include "TextItem.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace kubik
{
	namespace config
	{
		class StatBlock:public Dispatcher
		{
		public:

			StatBlock(ConfigSettingsRef configSettings, Vec2i position)
						:Dispatcher(),
						title1(configSettings->getTextItem(ConfigTextID::PLAYED_COUNT)),
						title2(configSettings->getTextItem(ConfigTextID::PRINTED_COUNT)),
						title3(configSettings->getTextItem(ConfigTextID::SOCIAL_COUNT)),						
						numsColor(Color::hex(0xffffff)),
						linesColor(Color::hex(0x233442)),
						numsFont(configSettings->getFont("introBold110"))
			{				
				setPosition(position);
			}

			virtual void drawLayout()
			{
				textTools().textFieldDraw(title1, Vec2f(-4, -4));
				textTools().textFieldDraw(title2, Vec2f(-4 + 350, -4));
				textTools().textFieldDraw(title3, Vec2f(-4 + 645, -4));	
				textTools().textFieldDraw(to_string(playedTimes), &numsFont, numsColor, Vec2f(-25, -4));	
				textTools().textFieldDraw(to_string(printedPhotos), &numsFont, numsColor, Vec2f(325, -4));	
				textTools().textFieldDraw(to_string(sharedAndEmail), &numsFont, numsColor, Vec2f(620, -4));	

				gl::lineWidth(2);
				gl::color(linesColor);
				gl::drawLine(Vec2f(0, 190), Vec2f(880, 190));
				gl::drawLine(Vec2f(880/3.0f, 0), Vec2f(880/3.0f, 190));
				gl::drawLine(Vec2f(2*880/3.0f, 0), Vec2f(2*880/3.0f, 190));
				gl::color(Color::white());					
			}		

			void setAlpha(float  alpha)
			{
				title1.setColor(Utils::colorAlpha(title1.getColor(), alpha));	
				title2.setColor(Utils::colorAlpha(title2.getColor(), alpha));	
				title3.setColor(Utils::colorAlpha(title3.getColor(), alpha));	
			
				linesColor = Utils::colorAlpha(linesColor, alpha);	
				numsColor = Utils::colorAlpha(numsColor, alpha);	
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

			TextItem title1, title2, title3;
			ColorA  linesColor, numsColor;

			int playedTimes;
			int printedPhotos;
			int sharedAndEmail;
		};

		typedef std::shared_ptr<StatBlock> StatBlockRef;
	}
}