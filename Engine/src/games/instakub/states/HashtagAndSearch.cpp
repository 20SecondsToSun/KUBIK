#include "instakub/states/HashtagAndSearch.h"

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


	title.setMagFilter(GL_NEAREST); // disable multi-sample if >= 100%
	title.setMinFilter(GL_LINEAR);  // enable multi-sampling if < 100%   

	searchField.setMagFilter(GL_NEAREST); // disable multi-sample if >= 100%
	searchField.setMinFilter(GL_LINEAR);  // enable multi-sampling if < 100%   

	titlePosition = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 176.0f - title.getHeight() * 0.5f);	
	searchFieldPosition = Vec2f(0.5f * (getWindowWidth() - searchField.getWidth()), 347.0f - searchField.getHeight() * 0.5f);

	hashtagPosition = Vec2f(252.0f, 319.0f);
}

void HashtagAndSearch::start()
{
	SearchLocation::start();
	hashTagAlpha = 1.0f;
	hashtagTexture = textTools().getTextField(
		HASH_SYMBOL + settings->getHashtag(), 
		&settings->getFont("introLight36"),
		Color::white());
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
		SearchLocation::closeKeyboardHandler();
}

void HashtagAndSearch::openPopupHandler()
{
	hashTagAlpha = 0.0f;
	SearchLocation::openPopupHandler();
}

void HashtagAndSearch::closePopupHandler()
{
	if(mode == HASHTAG_DEFAULT_PHOTOS_LOAD)	
		hashTagAlpha = 1.0f;

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