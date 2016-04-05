#include "instakub/states/SearchLocation.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

const ci::Vec2f SearchLocation::HIDING_KEYBORAD_POSITION  = Vec2f(30.0f, 1920.0f);
const ci::Vec2f SearchLocation::SHOWING_KEYBORAD_POSITION = Vec2f(30.0f, 595.0f);
const float SearchLocation::HIDING_KEYBORAD_TIME = 0.7f;
const float SearchLocation::SHOWING_KEYBORAD_TIME = 0.7f;

SearchLocation::SearchLocation(InstakubSettingsRef settings, const ci::Vec2f& vectr)
	:InstakubLocation(settings, vectr),	alphaError(0.0f)
{	
	
}

void SearchLocation::reset()
{
	InstakubLocation::reset();
	searchBtns = SearchButtonsRef(new SearchButtons(
		settings->getTexture("hashtagSearch"),
		settings->getTexture("hashtagSearchOver"),
		settings->getTexture("userSearch"),
		settings->getTexture("userSearchOver")));

	keyBackground = KeyBackgroundRef(new KeyBackground(Vec2f(0.0f, 515.0f), settings->getTexture("closeKeyboard"), settings->getKeyboardColorInDesign()));
}

void SearchLocation::start()
{
	touchKeyboard().setInputFieldText("");
	searchingText = "";
	
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
	//instaViewer->animatePositionTo(position.x, position.y);
	instaViewer->connect();
	hideKeyboardLayout();
	resetInputField();
}

void SearchLocation::resetInputField()
{
	touchKeyboard().setInputFieldText(searchingText);
}

void SearchLocation::initVirtualKeyboard()
{
	touchKeyboard().setDefaultSettings();
	touchKeyboard().setEraseButtonTexture(settings->getTexture("eraseInstagram"));
	touchKeyboard().clearInputFieldText();
	touchKeyboard().setOriginPoint(Vec2f::zero());
	touchKeyboard().setInputField(234.0f, 290.0f, 924.0f, 402.0f);
	touchKeyboard().setInputColor(Color::white());
	touchKeyboard().setInputFont(settings->getViewInputFieldFont());
	touchKeyboard().activateSearchMode();
}

void SearchLocation::inputTouchHandler()
{
	if (touchKeyboard().showing())
		return;

	string hashtag = touchKeyboard().getInputFieldText();

	instaViewer->disconnect();
	touchKeyboard().show(HIDING_KEYBORAD_POSITION, SHOWING_KEYBORAD_POSITION, SHOWING_KEYBORAD_TIME);
	keyBackground->show(EaseOutCubic(), SHOWING_KEYBORAD_TIME);
	searchBtns->show();
}

void SearchLocation::searchTouchHandler()
{
	if (!touchKeyboard().emptyInputField())
	{
		searchingText = touchKeyboard().getInputFieldText();

		if (searchBtns->hashtagSearchMode())
		{
			mode = HASHTAG_PHOTOS_LOAD;
		}
		else
		{
			mode = USER_PHOTOS_LOAD;
		}

		reload();
	}
	else
	{
		timeline().apply(&alphaError, 1.0f, 0.0f, 2.5f, EaseOutCubic());
	}
}

void SearchLocation::disconnectKeyboard()
{
	hideKeyboardLayout();
	disconnectKeyboardListeners();
}

void SearchLocation::hideKeyboardLayout()
{
	if (!touchKeyboard().showing())//todo!! while here?
		return;

	searchBtns->hide();
	touchKeyboard().hide(HIDING_KEYBORAD_POSITION, HIDING_KEYBORAD_TIME);
	keyBackground->hide(EaseOutCubic(), HIDING_KEYBORAD_TIME);
}

void SearchLocation::disconnectKeyboardListeners()
{
	searchBtns->disconnect();
	keyBackground->disconnectEventHandler(KeyBackground::HIDE_KEYBOARD);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::INPUT_TOUCH);
	touchKeyboard().disconnectEventHandler(VirtualKeyboard::SEARCH_TOUCH);
}

void SearchLocation::connectKeyboard()
{
	searchBtns->connect();
	touchKeyboard().connectKeyboard();
	touchKeyboard().connectEventHandler(&SearchLocation::inputTouchHandler, this, VirtualKeyboard::INPUT_TOUCH);
	touchKeyboard().connectEventHandler(&SearchLocation::searchTouchHandler, this, VirtualKeyboard::SEARCH_TOUCH);
	keyBackground->connectEventHandler(&SearchLocation::closeKeyboardHandler, this, KeyBackground::HIDE_KEYBOARD);
}

void SearchLocation::stop()
{
	InstakubLocation::stop();
	disconnectKeyboardListeners();
	disconnectKeyboard();
}

void SearchLocation::drawTouchKeyboardLayout()
{
	keyBackground->draw();
	gl::color(Color::white());
	touchKeyboard().draw();
	searchBtns->draw();
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

void SearchLocation::openPopupHandler()
{
	InstakubLocation::openPopupHandler();
	disconnectKeyboard();
}

void SearchLocation::closePopupHandler()
{
	InstakubLocation::closePopupHandler();
	connectKeyboard();
}

void SearchLocation::reload()
{
	if (touchKeyboard().showing())
	{
		instaViewer->connect();
		disconnectKeyboard();
	}
	
	InstakubLocation::reload();
}

void SearchLocation::loadStrategity()
{
	switch (mode)
	{
	case HASHTAG_PHOTOS_LOAD:
		hashTagPhotosLoad();
		instaViewer->setTopDragVisible(true);
		break;

	case HASHTAG_DEFAULT_PHOTOS_LOAD:
		hashTagDefaultPhotosLoad();
		instaViewer->setTopDragVisible(true);
		break;

	case USER_PHOTOS_LOAD:
		userPhotosLoad();
		instaViewer->setTopDragVisible(true);
		break;

	case POPULAR_PHOTOS_LOAD:
		popularPhotosLoad();
		instaViewer->setTopDragVisible(false);
		break;
	}
}

void SearchLocation::hashTagPhotosLoad()
{
	string hashtag = touchKeyboard().getInputFieldText();
	logger().log("LOAD STRAGEDY:::::::::::::::: HASHTAG_PHOTOS_LOAD ::::" + hashtag);
	hashtagPhotosload(hashtag);
}

void SearchLocation::userPhotosLoad()
{
	string userName = touchKeyboard().getInputFieldText();
	logger().log("LOAD STRAGEDY:::::::::::::::: USER_PHOTOS_LOAD :::: " + userName);
	userPhotosload(userName);
}

void SearchLocation::draw()
{
	InstakubLocation::draw();
	gl::color(Color::white());
	gl::draw(overMask);
	gl::draw(title, Vec2i(titlePosition.x, titlePosition.y));

	gl::draw(searchField, Vec2i(searchFieldPosition.x, searchFieldPosition.y));
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaError));

	gl::pushMatrices();	
	gl::draw(searchFieldRed, searchFieldPosition);
	gl::popMatrices();

	gl::color(Color::white());

	drawTouchKeyboardLayout();
	InstakubLocation::drawPopup();
}
