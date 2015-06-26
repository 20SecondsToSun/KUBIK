#include "main/statBlock/StatBlock.h"

using namespace ci;
using namespace kubik;
using namespace kubik::config;

StatBlock::StatBlock(ConfigSettingsRef configSettings, const Vec2i& position)
	:Sprite(),
	title1(configSettings->getTextItem(ConfigTextID::PLAYED_COUNT)),
	title2(configSettings->getTextItem(ConfigTextID::PRINTED_COUNT)),
	title3(configSettings->getTextItem(ConfigTextID::SOCIAL_COUNT)),
	numsColor(Color::white()),
	linesColor(Color::hex(0x233442)),
	numsFont1(getFont("IntroBold", 110)),
	numsFont2(getFont("IntroBold", 70)),
	numsFont3(getFont("IntroBold", 60)),
	playedTimes(0),
	printedPhotos(0),
	sharedAndEmail(0)
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
	gl::color(Color::hex(0x0d0917));
	gl::drawSolidRect(Rectf(0.0f, 0.0f, 850.0f, 190.0f));
	gl::color(Color::white());
	textTools().textFieldDraw(title1, Vec2f(-4.0f, -4.0f));
	textTools().textFieldDraw(title2, Vec2f(-4.0f + 350.0f, -4.0f));
	textTools().textFieldDraw(title3, Vec2f(-4.0f + 645.0f, -4.0f));

	gl::color(numsColor);
	gl::draw(playedTimesTexture,	playedTimesPosition);
	gl::draw(printedPhotosTexture,  printedPhotosPosition);
	gl::draw(sharedAndEmailTexture, sharedAndEmailPosition);

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
	numsColor  = Utils::colorAlpha(numsColor,  alpha);
}

void StatBlock::setPlayedTimes(int num)
{
	playedTimes = num;
	calculateDigitTextures();
}

void StatBlock::setPrintedPhotos(int num)
{
	printedPhotos = num;
	calculateDigitTextures();
}

void StatBlock::setSharedAndEmail(int num)
{
	sharedAndEmail = num;
	calculateDigitTextures();
}

void StatBlock::nullValues()
{
	playedTimes = 0;
	printedPhotos = 0;
	sharedAndEmail = 0;
	calculateDigitTextures();
}

void StatBlock::calculateDigitTextures()
{
	auto chooseFont = [&](int count)->ci::Font
	{
		auto str = to_string(count);
		ci::Font font = numsFont1;
	
		if (str.length() <= 3)
			font  = numsFont1;
		else if (str.length() <= 4)
			font  = numsFont2;
		else 
			font  = numsFont3;

		return font;
	};

	playedTimesTexture    = textTools().getTextField(to_string(playedTimes), &chooseFont(playedTimes), numsColor);
	printedPhotosTexture  = textTools().getTextField(to_string(printedPhotos), &chooseFont(printedPhotos), numsColor);
	sharedAndEmailTexture = textTools().getTextField(to_string(sharedAndEmail), &chooseFont(sharedAndEmail), numsColor);

	auto getYPosition = [](int count)->Vec2f
	{
		auto str = to_string(count);
		auto position = Vec2f::zero();

		if (str.length() <= 3)
			position = Vec2f(0.0f, -4.0f);
		else if (str.length() <= 4)
			position = Vec2f(10.0f, 30.0f);
		else 
			position = Vec2f(15.0f, 40.0f);

		return position;
	};

	playedTimesPosition    = getYPosition(playedTimes)    + Vec2f(-25.0f, 0.0f );
	printedPhotosPosition  = getYPosition(printedPhotos)  + Vec2f(325.0f, 0.0f );
	sharedAndEmailPosition = getYPosition(sharedAndEmail) + Vec2f(620.0f, 0.0f );
}