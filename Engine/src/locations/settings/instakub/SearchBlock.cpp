#include "SearchBlock.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

SearchBlock::SearchBlock(InstakubSettingsRef settings, const Vec2i& position):Sprite()
{
	setPosition(position);
	
	titleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::SEARCH_TITLE_MAIN));
	titleTextPos = Vec2f(0.5f * (914.0f - titleTextTex.getWidth()), 100.0f);			

	subTitleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::SEARCH_TITLE_SUB));
	subTitleTextPos = Vec2f(0.5f * (914.0f - subTitleTextTex.getWidth()), 166.0f);

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
	//mouseUpSignal(event);
}

void SearchBlock::drawLayout()
{
	gl::color(Color::hex(0xdc831a));
	gl::drawSolidRect(Rectf(Vec2f::zero(), Vec2f(914.0f, 435.0f)));
	gl::color(Color::white());

	gl::draw(titleTextTex, titleTextPos);
	gl::draw(subTitleTextTex, subTitleTextPos);				
}

bool SearchBlock::isChecked()
{
	return checker->getValue();
}