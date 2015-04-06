#include "instakub/states/HashtagAndSearch.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

HashtagAndSearch::HashtagAndSearch(InstakubSettingsRef settings) :InstakubLocation(settings, Vec2f(18.0f, 518.0f))
{	
	reset();
}

void HashtagAndSearch::start()
{
	
}

void HashtagAndSearch::load()
{
	InstakubLocation::start();
	InstakubLocation::load();
}

void HashtagAndSearch::stop()
{
	InstakubLocation::stop();	
}

void HashtagAndSearch::reset()
{
	InstakubLocation::reset();
	InstakubLocation::initOverMask();

	title = settings->getTexture("searchTitle");
	titlePosition = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 176.0f - title.getHeight() * 0.5f);
	searchField = settings->getTexture("searchField");
	searchFieldPosition = Vec2f(0.5f * (getWindowWidth() - searchField.getWidth()), 357.0f - searchField.getHeight() * 0.5f);
}

void HashtagAndSearch::draw()
{
	InstakubLocation::draw();	
	gl::color(Color::white());
	gl::draw(overMask);
	gl::draw(title, titlePosition);
	gl::draw(searchField, searchFieldPosition);	
	InstakubLocation::drawPopup();
};