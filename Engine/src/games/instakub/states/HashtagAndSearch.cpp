#include "instakub/states/HashtagAndSearch.h"

using namespace ci;
using namespace ci::app;
using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

HashtagAndSearch::HashtagAndSearch(InstakubSettingsRef settings)
	:SearchLocation(settings, Vec2f(18.0f, 515.0f))
{
	reset();
}

void HashtagAndSearch::load()
{
	mode = HASHTAG_DEFAULT_PHOTOS_LOAD;
	SearchLocation::load();	
	loadStrategity();
}

void HashtagAndSearch::reset()
{
	SearchLocation::reset();
	InstakubLocation::initOverMask();

	title = settings->getTexture("searchTitle");	
	searchField = settings->getTexture("searchField");
	searchFieldRed = settings->getTexture("searchFieldError");	

	titlePosition = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 176.0f - title.getHeight() * 0.5f);	
	searchFieldPosition = Vec2f(0.5f * (getWindowWidth() - searchField.getWidth()), 347.0f - searchField.getHeight() * 0.5f);
}

void HashtagAndSearch::start()
{
	SearchLocation::start();
	hashTagAlpha = 1.0f;
	hashtagTexture = textTools().getTextField(
		HASH_SYMBOL + settings->getHashtag().getText(), 
		&settings->getViewInputFieldFont(),
		Color::white());

	hashtagPosition = Vec2f(252.0f, searchFieldPosition.y + (searchField.getHeight() - hashtagTexture.getHeight()) * 0.5f);
}

void HashtagAndSearch::inputTouchHandler()
{
	hashTagAlpha = 0.0f;
	SearchLocation::inputTouchHandler();
}

void HashtagAndSearch::closeKeyboardHandler()
{
	if (touchKeyboard().emptyInputField() || searchingText =="")
	{		
		instaViewer->connect();
		hideKeyboardLayout();	
		showDefaultHashtag();		
		reload();
	}
	else
	{
		SearchLocation::closeKeyboardHandler();
	}		
}

void HashtagAndSearch::openPopupHandler()
{
	hashTagAlpha = 0.0f;
	SearchLocation::openPopupHandler();
}

void HashtagAndSearch::closePopupHandler()
{
	if (mode == HASHTAG_DEFAULT_PHOTOS_LOAD)
	{
		hashTagAlpha = 1.0f;
	}		

	SearchLocation::closePopupHandler();
}

void HashtagAndSearch::draw()
{
	SearchLocation::draw();
	gl::color(ColorA(1.0f, 1.0f, 1.0f, hashTagAlpha));
	gl::draw(hashtagTexture, hashtagPosition);
	gl::color(Color::white());
};

void HashtagAndSearch::timeOutReload()
{
	showDefaultHashtag();
	SearchLocation::reload();
}

void HashtagAndSearch::showDefaultHashtag()
{
	hashTagAlpha = 1.0f;
	mode = HASHTAG_DEFAULT_PHOTOS_LOAD;
	touchKeyboard().setInputFieldText("");
}