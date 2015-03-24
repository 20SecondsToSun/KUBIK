#include "main/statBlock/StatBlock.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;

StatBlock::StatBlock(ConfigSettingsRef configSettings, const Vec2i& position)
	:Sprite(),
	title1(configSettings->getTextItem(ConfigTextID::PLAYED_COUNT)),
	title2(configSettings->getTextItem(ConfigTextID::PRINTED_COUNT)),
	title3(configSettings->getTextItem(ConfigTextID::SOCIAL_COUNT)),
	numsColor(ci::Color::hex(0xffffff)),
	linesColor(ci::Color::hex(0x233442)),
	numsFont(configSettings->getFont("introBold110"))
{
	setPosition(position);
	line1.point1 = Vec2f(0.0f, 190.0f);
	line1.point2 = Vec2f(880.0f, 190.0f);

	line2.point1 = Vec2f(880.0f / 3.0f, 0.0f);
	line2.point2 = Vec2f(880.0f / 3.0f, 190.0f);

	line3.point1 = Vec2f(2.0f * 880.0f / 3.0f, 0.0f);
	line3.point2 = Vec2f(2.0f * 880.0f / 3.0f, 190.0f);
}

void StatBlock::drawLayout()
{
	textTools().textFieldDraw(title1, Vec2f(-4.0f, -4.0f));
	textTools().textFieldDraw(title2, Vec2f(-4.0f + 350.0f, -4.0f));
	textTools().textFieldDraw(title3, Vec2f(-4.0f + 645.0f, -4.0f));
	textTools().textFieldDraw(to_string(playedTimes), &numsFont, numsColor, Vec2f(-25.0f, -4.0f));
	textTools().textFieldDraw(to_string(printedPhotos), &numsFont, numsColor, Vec2f(325.0f, -4.0f));
	textTools().textFieldDraw(to_string(sharedAndEmail), &numsFont, numsColor, Vec2f(620.0f, -4.0f));

	gl::lineWidth(2);
	gl::color(linesColor);
	gl::drawLine(line1.point1, line1.point2);
	gl::drawLine(line2.point1, line2.point2);
	gl::drawLine(line3.point1, line3.point2);
	gl::color(Color::white());
}

void StatBlock::setAlpha(float alpha)
{
	title1.setColor(Utils::colorAlpha(title1.getColor(), alpha));
	title2.setColor(Utils::colorAlpha(title2.getColor(), alpha));
	title3.setColor(Utils::colorAlpha(title3.getColor(), alpha));

	linesColor = Utils::colorAlpha(linesColor, alpha);
	numsColor = Utils::colorAlpha(numsColor, alpha);
}

void StatBlock::setPlayedTimes(int num)
{
	playedTimes = num;
}

void StatBlock::setPrintedPhotos(int num)
{
	printedPhotos = num;
}

void StatBlock::setSharedAndEmail(int num)
{
	sharedAndEmail = num;
}

void StatBlock::nullValues()
{
	playedTimes = 0;
	printedPhotos = 0;
	sharedAndEmail = 0;
}