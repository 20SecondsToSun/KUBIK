#include "instakub/states/SearchOnly.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

SearchOnly::SearchOnly(InstakubSettingsRef settings)
	:SearchLocation(settings, Vec2f(18.0f, 515.0f))
{	
	reset();
}

void SearchOnly::load()
{
	mode = POPULAR_PHOTOS_LOAD;
	SearchLocation::load();	
	loadStrategity();
}

void SearchOnly::reset()
{
	SearchLocation::reset();
	InstakubLocation::initOverMask();

	title		= settings->getTexture("searchTitle");
	searchField = settings->getTexture("searchField");
	searchFieldRed = settings->getTexture("searchFieldError");
	titlePosition = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 176.0f - title.getHeight() * 0.5f);	
	searchFieldPosition = Vec2f(0.5f * (getWindowWidth() - searchField.getWidth()), 347.0f - searchField.getHeight() * 0.5f);
}

void SearchOnly::draw()
{
	SearchLocation::draw();
}

void SearchOnly::timeOutReload()
{
	mode = POPULAR_PHOTOS_LOAD;
	touchKeyboard().setInputFieldText("");
	SearchLocation::reload();
}