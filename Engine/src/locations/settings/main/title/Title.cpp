#include "main/title//Title.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;

Title::Title(ConfigSettingsRef configSettings, const Vec2i& position)
	:Sprite(),
	name(configSettings->getActionName()),
	font(configSettings->getFont("introLight44")),
	color(Color::white())
{
	setPosition(position);
}

void Title::drawLayout()
{
	gl::color(Color::hex(0x0d0917));
	gl::drawSolidRect(Rectf(0, -100, 850, 190));
	gl::color(Color::white());
	textTools().textFieldDraw(name, &font, color, Vec2f(-12.0f, 3.0f));
}

void Title::setAlpha(float alpha)
{
	color = Utils::colorAlpha(color, alpha);
}

void Title::setActivityName(const std::string& name)
{
	this->name = name;
}