#include "instakub/states/SearchOnly.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

SearchOnly::SearchOnly(InstakubSettingsRef settings)
	:SearchLocation(settings, Vec2f(18.0f, 518.0f))	
{	
	reset();
}

void SearchOnly::start()
{
	SearchLocation::start();
}

void SearchOnly::load()
{
	SearchLocation::load();	
	loadStrategity();
}

void SearchOnly::loadStrategity()
{
	InstakubLocation::loadPopuplar();
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

void SearchOnly::reload()
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