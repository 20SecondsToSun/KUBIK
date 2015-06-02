#include "SearchBlock.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

SearchBlock::SearchBlock(InstakubSettingsRef settings, const Vec2i& position)
	:Sprite(),
	backgroundColor(Color::hex(0xdc831a)),
	titleTextPos(Vec2f(0.0f, 70.0f)),
	subTitleTextPos(Vec2f(0.0f, 145.0f))
{
	setPosition(position);

	titleItem = settings->getTextItem(InstakubSettings::InstaTextID::SEARCH_TITLE_MAIN);
	subTitleItem = settings->getTextItem(InstakubSettings::InstaTextID::SEARCH_TITLE_SUB);	

	IconPair icons(settings->getTexture("checkerw"), settings->getTexture("checkerw"));
	checker = HashCheckerRef(new HashChecker(Rectf( Vec2f::zero(), Vec2f(131.0f, 78.0f)), icons));				
	checker->setActive(settings->getSearchFlag());
	checker->setPosition(Vec2f(390.0f, 243.0f));
	addChild(checker);
}	

void SearchBlock::activateListeners()
{
	checker->connectEventHandler(&SearchBlock::checkerClicked, this);
}

void SearchBlock::unActivateListeners()
{
	checker->disconnectEventHandler();
}

void SearchBlock::checkerClicked(EventGUIRef& event)
{

}

void SearchBlock::drawLayout()
{
	gl::color(backgroundColor);
	gl::drawSolidRect(Rectf(Vec2f::zero(), Vec2f(914.0f, 435.0f)));
	gl::color(Color::white());

	textTools().drawTextBox(titleItem, backgroundColor, titleTextPos, Vec2i(914.0f, 100.0f));
	textTools().drawTextBox(subTitleItem, backgroundColor, subTitleTextPos, Vec2i(914.0f, 50.0f));
}

bool SearchBlock::isChecked()
{
	return checker->getValue();
}