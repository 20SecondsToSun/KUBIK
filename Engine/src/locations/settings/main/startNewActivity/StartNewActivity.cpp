#include "main/startNewActivity/StartNewActivity.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;

StartNewActivity::StartNewActivity(ConfigSettingsRef configSettings, const Vec2i& position)
	:SimpleSpriteButton(Rectf(0.0f, 0.0f, 350.0f, 37.0f)),
	icon(configSettings->getTexture("iconStartNew")),
	textItem(configSettings->getTextItem(ConfigTextID::FINANDSTART)),
	color(configSettings->getTextItem(ConfigTextID::FINANDSTART).getColor())
{
	setPosition(position);
}

void StartNewActivity::drawLayout()
{
	gl::color(color);
	gl::draw(icon);
	textTools().textFieldDraw(textItem, Vec2f(42.0f, -1.0f));
}

void StartNewActivity::setAlpha(float alpha)
{
	color = Utils::colorAlpha(color, alpha);
}