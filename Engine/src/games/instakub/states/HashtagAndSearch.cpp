#include "instakub/states/HashtagAndSearch.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

HashtagAndSearch::HashtagAndSearch(InstakubSettingsRef settings)
	:SearchLocation(settings, Vec2f(18.0f, 518.0f))
{
	reset();
}

void HashtagAndSearch::start()
{
	SearchLocation::start();
}

void HashtagAndSearch::load()
{
	SearchLocation::load();
	loadStrategity();
}

void HashtagAndSearch::loadStrategity()
{
	string hashtag = settings->getHashtag();
	InstakubLocation::hashTagOnlyload(hashtag);
}

void HashtagAndSearch::reset()
{
	InstakubLocation::reset();
	InstakubLocation::initOverMask();

	title = settings->getTexture("searchTitle");
	searchField = settings->getTexture("searchField");
	searchFieldRed = settings->getTexture("searchFieldError");

	titlePosition = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 176.0f - title.getHeight() * 0.5f);	
	searchFieldPosition = Vec2f(0.5f * (getWindowWidth() - searchField.getWidth()), 357.0f - searchField.getHeight() * 0.5f);
}

void HashtagAndSearch::draw()
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
};

void HashtagAndSearch::reload()
{
	if (instaPopup->isOpen())
		closePopupHandler();

	if (touchKeyboard().showing())
	{
		instaViewer->connect();
		disconnectKeyboard();
	}

	if (mode != POPULAR_MODE)
	{
		console() << "RELOAD hashtag" << endl;
		InstakubLocation::reload();
		hashTagOnlyload(touchKeyboard().getInputFieldText());
	}
}