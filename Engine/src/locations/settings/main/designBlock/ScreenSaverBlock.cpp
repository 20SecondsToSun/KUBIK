#include "main/designBlock/ScreenSaverBlock.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

ScreenSaverBlock::ScreenSaverBlock(ConfigSettingsRef configSettings, ScreenSaverSettingsRef ssSettings, const Vec2i& position)
	:Sprite(),
	configSettings(configSettings),
	lineColor(ci::Color::hex(0x233442)),
	lineWidth(2),
	lineLength(707.0f)
{
	setPosition(position);

	loadButton = settingsFactory().createLoadButton(ssSettings->getPath(), Vec2f(505.0f, 66.0f));
	addChild(loadButton);

	checker = settingsFactory().createScreenSaverChecker(Vec2f(0.0f, 60.0f));
	checker->setActive(ssSettings->getActive());
	addChild(checker);
}

bool ScreenSaverBlock::getScreenSaverValue() const
{
	return checker->getValue();
}

void ScreenSaverBlock::activateListeners()
{
	checker->connectEventHandler(&ScreenSaverBlock::checkerClicked, this);
	loadButton->connectEventHandler(&ScreenSaverBlock::openDirectory, this);
}

void ScreenSaverBlock::unActivateListeners()
{
	checker->disconnectEventHandler();
	loadButton->disconnectEventHandler();
}

void ScreenSaverBlock::checkerClicked(EventGUIRef event)
{
	callback(SCREEN_SAVER_STATE);
}

void ScreenSaverBlock::openDirectory(EventGUIRef event)
{
	callback(SCREEN_SAVER_OPEN_FOLDER);
}

void ScreenSaverBlock::drawLayout()
{
	drawDecorationLine();
	//gl::drawSolidRect(Rectf(0.0f, 0.0f, 707.0f, 200.0f));

	gl::pushMatrices();
	gl::translate(0, 200);
	drawDecorationLine();
	gl::popMatrices();

	textTools().textFieldDraw(
		configSettings->getTextItem(ConfigTextID::LOADMAIN),
		ci::Vec2f(171.0f, 57.0f));

	textTools().textFieldDraw(
		configSettings->getTextItem(ConfigTextID::LOADSUB),
		ci::Vec2f(171.0f, 105.0f));

	gl::color(ci::Color::white());
}

void ScreenSaverBlock::drawDecorationLine()
{
	gl::lineWidth(lineWidth);
	gl::color(lineColor);
	gl::drawLine(Vec2f::zero(), Vec2f(lineLength, 0.0f));
	gl::color(Color::white());
}

void ScreenSaverBlock::setAlpha(float alpha)
{
	//textcolor = Utils::colorAlpha(textcolor, alpha);	
}