#include "instakub/states/SearchLocation.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

const ci::Vec2f SearchLocation::HIDING_KEYBORAD_POSITION = Vec2f(30.0f, 1920.0f);
const ci::Vec2f SearchLocation::SHOWING_KEYBORAD_POSITION = Vec2f(30.0f, 595.0f);
const float SearchLocation::HIDING_KEYBORAD_TIME = 0.7f;
const float SearchLocation::SHOWING_KEYBORAD_TIME = 0.7f;

SearchLocation::SearchLocation(InstakubSettingsRef settings, const ci::Vec2f& vectr)
	:InstakubLocation(settings, vectr),
	alphaError(0.0f)
{	
	keyBackground = KeyBackgroundRef(new KeyBackground(Vec2f(0.0f, 515.0f)));
}

void SearchLocation::start()
{
	touchKeyboard().setInputFieldText("");
	InstakubLocation::initPosition();
	InstakubLocation::start();
	alphaKeyBoardBg = 0.0f;
}

void SearchLocation::load()
{
	initVirtualKeyboard();
	InstakubLocation::start();
}

void SearchLocation::closeKeyboardHandler()
{
	instaViewer->connect();
	touchKeyboard().hide(HIDING_KEYBORAD_POSITION, HIDING_KEYBORAD_TIME);
	keyBackground->hide(EaseOutCubic(), HIDING_KEYBORAD_TIME);

	if (touchKeyboard().emptyInputField())
	{
		clear();
		instaViewer->showPreloader();
		loadStrategity();
	}
}

void SearchLocation::initVirtualKeyboard()
{
	touchKeyboard().clearInputFieldText();
	touchKeyboard().setOriginPoint(Vec2f::zero());
	touchKeyboard().setInputField(258.0f, 335.0f, 895.0f, 386.0f);
	touchKeyboard().setInputColor(Color::white());
	touchKeyboard().activateSearchMode();
}

void SearchLocation::inputTouchHandler()
{
	instaViewer->disconnect();
	touchKeyboard().show(HIDING_KEYBORAD_POSITION, SHOWING_KEYBORAD_POSITION, SHOWING_KEYBORAD_TIME);
	keyBackground->show(EaseOutCubic(), SHOWING_KEYBORAD_TIME);
}

void SearchLocation::searchTouchHandler()
{
	if (!touchKeyboard().emptyInputField())
	{
		mode = HASHTAG_MODE;
		closeKeyboardHandler();
		reload();
	}
	else	
		timeline().apply(&alphaError, 1.0f, 0.0f, 2.5f, EaseOutCubic());	
}

void SearchLocation::disconnectKeyboard()
{
	touchKeyboard().hide(HIDING_KEYBORAD_POSITION, HIDING_KEYBORAD_TIME);
	keyBackground->hide(EaseOutCubic(), HIDING_KEYBORAD_TIME);
	keyBackground->disconnectEventHandler(KeyBackground::HIDE_KEYBOARD);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::SEARCH_TOUCH);
}

void SearchLocation::connectKeyboard()
{
	touchKeyboard().connectKeyboard();
	touchKeyboard().connectEventHandler(&SearchLocation::inputTouchHandler, this, VirtualKeyboard::INPUT_TOUCH);
	touchKeyboard().connectEventHandler(&SearchLocation::searchTouchHandler, this, VirtualKeyboard::SEARCH_TOUCH);
	keyBackground->connectEventHandler(&SearchLocation::closeKeyboardHandler, this, KeyBackground::HIDE_KEYBOARD);
}

void SearchLocation::stop()
{
	InstakubLocation::stop();
	keyBackground->disconnectEventHandler(KeyBackground::HIDE_KEYBOARD);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::SEARCH_TOUCH);
}

void SearchLocation::drawTouchKeyboardLayout()
{
	keyBackground->draw();
	gl::color(Color::white());
	touchKeyboard().draw();
}

void SearchLocation::openPopupHandler()
{
	InstakubLocation::openPopupHandler();
	disconnectKeyboard();
}

void SearchLocation::loadingCompleteHandler()
{
	InstakubLocation::loadingCompleteHandler();
	connectKeyboard();
}

void SearchLocation::startLoadHandler()
{
	InstakubLocation::startLoadHandler();
	disconnectKeyboard();
}

void SearchLocation::noMoreLoadsHandler()
{
	InstakubLocation::noMoreLoadsHandler();
	connectKeyboard();
}

void SearchLocation::closePopupHandler()
{
	InstakubLocation::closePopupHandler();
	connectKeyboard();
}