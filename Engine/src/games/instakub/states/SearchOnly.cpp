#include "instakub/states/SearchOnly.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

SearchOnly::SearchOnly(InstakubSettingsRef settings) :InstakubLocation(settings, Vec2f(18.0f, 518.0f))
{	
	reset();
}

void SearchOnly::start()
{
	InstakubLocation::initPosition();	
}

void SearchOnly::load()
{
	InstakubLocation::start();
	InstakubLocation::load();
}

void SearchOnly::stop()
{
	InstakubLocation::stop();
}

void SearchOnly::reset()
{
	InstakubLocation::reset();
	InstakubLocation::initOverMask();

	title = settings->getTexture("searchTitle");
	titlePosition = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 176.0f - title.getHeight() * 0.5f);
	searchField = settings->getTexture("searchField");
	searchFieldPosition = Vec2f(0.5f * (getWindowWidth() - searchField.getWidth()), 357.0f - searchField.getHeight() * 0.5f);	
}

void SearchOnly::draw()
{
	InstakubLocation::draw();
	gl::draw(title, titlePosition);
	gl::draw(searchField, searchFieldPosition);	
};