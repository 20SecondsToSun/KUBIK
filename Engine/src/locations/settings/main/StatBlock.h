#pragma once
#include "gui/Sprite.h"
#include "TextItem.h"

namespace kubik
{
	namespace config
	{
		class StatBlock:public Sprite
		{
		public:

			StatBlock(ConfigSettingsRef configSettings, ci::Vec2i position)
						:Sprite(),
						title1(configSettings->getTextItem(ConfigTextID::PLAYED_COUNT)),
						title2(configSettings->getTextItem(ConfigTextID::PRINTED_COUNT)),
						title3(configSettings->getTextItem(ConfigTextID::SOCIAL_COUNT)),						
						numsColor(ci::Color::hex(0xffffff)),
						linesColor(ci::Color::hex(0x233442)),
						numsFont(configSettings->getFont("introBold110"))
			{				
				setPosition(position);
				line1.point1 = ci::Vec2f(0, 190);
				line1.point2 = ci::Vec2f(880, 190);

				line2.point1 = ci::Vec2f(880/3.0f, 0);
				line2.point2 = ci::Vec2f(880/3.0f, 190);

				line3.point1 = ci::Vec2f(2*880/3.0f, 0);
				line3.point2 =  ci::Vec2f(2*880/3.0f, 190);
			}

			virtual void drawLayout()
			{
				textTools().textFieldDraw(title1, ci::Vec2f(-4, -4));
				textTools().textFieldDraw(title2, ci::Vec2f(-4 + 350, -4));
				textTools().textFieldDraw(title3, ci::Vec2f(-4 + 645, -4));	
				textTools().textFieldDraw(to_string(playedTimes), &numsFont, numsColor, ci::Vec2f(-25, -4));	
				textTools().textFieldDraw(to_string(printedPhotos), &numsFont, numsColor, ci::Vec2f(325, -4));	
				textTools().textFieldDraw(to_string(sharedAndEmail), &numsFont, numsColor, ci::Vec2f(620, -4));	

				gl::lineWidth(2);
				gl::color(linesColor);
				gl::drawLine(line1.point1, line1.point2);
				gl::drawLine(line2.point1, line2.point2);
				gl::drawLine(line3.point1, line3.point2);				
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
			ci::Font numsFont;

			TextItem title1, title2, title3;
			ci::ColorA linesColor, numsColor;

			int playedTimes;
			int printedPhotos;
			int sharedAndEmail;

			Line<ci::Vec2f, ci::Vec2f> line1, line2, line3;
		};

		typedef std::shared_ptr<StatBlock> StatBlockRef;
	}
}