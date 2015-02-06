#pragma once
#include "Checker.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ScreenSaverChecker> ScreenSaverCheckerRef;

		class ScreenSaverChecker: public Checker
		{
		public:
			ScreenSaverChecker(Rectf rect, IconPair icons):
				Checker(rect, icons, Color::hex(0x00b6c4), Color::hex(0x08060f))				
			{
				setRadius(40);
				setStartX(-6);
				setFinishX(48);				
			}	

			virtual void drawLayout()
			{
				gl::color(color);
				gl::drawSolidRoundedRect(buttonArea, radius, 200);
				gl::color(Color::hex(0x242135));
				gl::lineWidth(3);
				gl::drawStrokedRoundedRect(buttonArea, radius, 200);
				gl::color(Color::white());
				gl::lineWidth(1);

				gl::pushMatrices();		
				gl::translate(sdvigX + buttonArea.x1, buttonArea.y1);
				gl::color(iconColor);
				gl::draw(icon);
				gl::popMatrices();				
			}
		};
	}
}