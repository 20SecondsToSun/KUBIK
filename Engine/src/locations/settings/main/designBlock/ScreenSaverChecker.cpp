#include "main/designBlock/ScreenSaverChecker.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

ScreenSaverChecker::ScreenSaverChecker(const Rectf& rect, const IconPair& icons) 
	:Checker(rect, icons, Color::hex(0x00b6c4), Color::hex(0x08060f))
{
	setRadius(40);
	setStartX(-6);
	setFinishX(48);
}

void ScreenSaverChecker::drawLayout()
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