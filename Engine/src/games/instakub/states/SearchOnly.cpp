#include "instakub/states/SearchOnly.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

const ci::Vec2f SearchOnly::HIDING_KEYBORAD_POSITION = Vec2f(30.0f, 1920.0f);
const ci::Vec2f SearchOnly::SHOWING_KEYBORAD_POSITION = Vec2f(30.0f, 595.0f);
const float SearchOnly::HIDING_KEYBORAD_TIME = 0.7f;
const float SearchOnly::SHOWING_KEYBORAD_TIME = 0.7f;

SearchOnly::SearchOnly(InstakubSettingsRef settings)
	:InstakubLocation(settings, Vec2f(18.0f, 518.0f)), alphaError(0.0f)
{	
	reset();
	keyBackground = KeyBackgroundRef(new KeyBackground(Vec2f(0.0f, 515.0f)));	
}

void SearchOnly::start()
{
	InstakubLocation::initPosition();
	InstakubLocation::start();
	alphaKeyBoardBg = 0.0f;
}

void SearchOnly::load()
{
	InstakubLocation::start();
	InstakubLocation::loadPopuplar();
	initVirtualKeyboard();
	//InstakubLocation::load();
}

void SearchOnly::initVirtualKeyboard()
{
	touchKeyboard().clearInputFieldText();
	touchKeyboard().setOriginPoint(Vec2f::zero());
	touchKeyboard().setInputField(258.0f, 335.0f, 895.0f, 386.0f);	
	touchKeyboard().setInputColor(Color::white());
	touchKeyboard().activateSearchMode();
}

void SearchOnly::inputTouchHandler()
{
	instaViewer->disconnect();
	touchKeyboard().show(HIDING_KEYBORAD_POSITION, SHOWING_KEYBORAD_POSITION, SHOWING_KEYBORAD_TIME);
	keyBackground->show(EaseOutCubic(), SHOWING_KEYBORAD_TIME);
}

void SearchOnly::searchTouchHandler()
{
	if (!touchKeyboard().emptyInputField())
	{
		mode = HASHTAG_MODE;
		closeKeyboardHandler();		
		reload();
	}
	else
	{
		timeline().apply(&alphaError, 1.0f, 0.0f, 2.5f, EaseOutCubic());
	}		
}

void SearchOnly::closeKeyboardHandler()
{
	instaViewer->connect();
	touchKeyboard().hide(HIDING_KEYBORAD_POSITION, HIDING_KEYBORAD_TIME);
	keyBackground->hide(EaseOutCubic(), HIDING_KEYBORAD_TIME);

	if (touchKeyboard().emptyInputField())
	{	
		mode = POPULAR_MODE;
		clear();
		instaViewer->showPreloader();
		InstakubLocation::loadPopuplar();
	}
}

void SearchOnly::disconnectKeyboard()
{
	touchKeyboard().hide(HIDING_KEYBORAD_POSITION, HIDING_KEYBORAD_TIME);
	keyBackground->hide(EaseOutCubic(), HIDING_KEYBORAD_TIME);
	keyBackground->disconnectEventHandler(KeyBackground::HIDE_KEYBOARD);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::SEARCH_TOUCH);
}

void SearchOnly::connectKeyboard()
{
	touchKeyboard().connectKeyboard();
	touchKeyboard().connectEventHandler(&SearchOnly::inputTouchHandler, this, VirtualKeyboard::INPUT_TOUCH);
	touchKeyboard().connectEventHandler(&SearchOnly::searchTouchHandler, this, VirtualKeyboard::SEARCH_TOUCH);
	keyBackground->connectEventHandler(&SearchOnly::closeKeyboardHandler, this, KeyBackground::HIDE_KEYBOARD);
}

void SearchOnly::stop()
{
	InstakubLocation::stop();
	keyBackground->disconnectEventHandler(KeyBackground::HIDE_KEYBOARD);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::SEARCH_TOUCH);
}

void SearchOnly::reset()
{
	InstakubLocation::reset();
	InstakubLocation::initOverMask();

	title		= settings->getTexture("searchTitle");
	searchField = settings->getTexture("searchField");
	searchFieldRed = settings->getTexture("searchFieldError");
	titlePosition = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 176.0f - title.getHeight() * 0.5f);	
	searchFieldPosition = Vec2f(0.5f * (getWindowWidth() - searchField.getWidth()), 357.0f - searchField.getHeight() * 0.5f);	
}

void SearchOnly::draw()
{
	InstakubLocation::draw();
	gl::color(Color::white());
	gl::draw(overMask);
	gl::draw(title, titlePosition);

	gl::draw(searchField, searchFieldPosition);
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaError));
	gl::draw(searchFieldRed, searchFieldPosition);
	gl::color(Color::white());	

	drawTouchKeyboardLayout();	
	InstakubLocation::drawPopup();
}

void SearchOnly::drawTouchKeyboardLayout()
{
	keyBackground->draw();
	gl::color(Color::white());
	touchKeyboard().draw();
}

void SearchOnly::reload()
{
	if (instaPopup->isOpen())
		closePopupHandler();

	if (touchKeyboard().showing())
	{
		instaViewer->connect();
		disconnectKeyboard();
	}		

	if (mode == POPULAR_MODE)
	{

	}
	else
	{
		console() << "RELOAD hashtag" << endl;
		InstakubLocation::reload();
		hashTagOnlyload(touchKeyboard().getInputFieldText());
	}
}

void SearchOnly::openPopupHandler()
{
	InstakubLocation::openPopupHandler();
	disconnectKeyboard();
}

void SearchOnly::loadingCompleteHandler()
{
	InstakubLocation::loadingCompleteHandler();
	connectKeyboard();
}

void SearchOnly::startLoadHandler()
{
	InstakubLocation::startLoadHandler();
	disconnectKeyboard();
}

void SearchOnly::noMoreLoadsHandler()
{
	InstakubLocation::noMoreLoadsHandler();
	connectKeyboard();
}

void SearchOnly::closePopupHandler()
{
	InstakubLocation::closePopupHandler();
	connectKeyboard();
}