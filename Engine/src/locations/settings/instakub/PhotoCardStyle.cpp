#include "PhotoCardStyle.h"
using namespace kubik::config;
using namespace kubik;
using namespace ci;

PhotoCardStyle::PhotoCardStyle(InstakubSettingsRef settings, const ci::Vec2i& position):Sprite()
{
	setPosition(position);	

	titleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::PHOTO_TITLE_MAIN));
	titleTextPos = Vec2f(0.5f * (914.0f - titleTextTex.getWidth()), 100.0f);			

	subTitleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::PHOTO_TITLE_SUB));
	subTitleTextPos = Vec2f(0.5f * (914.0f - subTitleTextTex.getWidth()), 166.0f);	

	sixBtnLayer = SixButtonsLayerRef( new SixButtonsLayer(settings));
	addChild(sixBtnLayer);
}

void PhotoCardStyle::drawLayout()
{
	drawBackground();
	drawTitles();
	Sprite::drawLayout();
}	

void PhotoCardStyle::drawBackground()
{
	gl::color(Color::hex(0x578d24));
	gl::drawSolidRect(Rectf(Vec2f::zero(), Vec2f(914.0f, 1000.0f)));
	gl::color(Color::white());
}

void PhotoCardStyle::drawTitles()
{
	gl::draw(titleTextTex, titleTextPos);
	gl::draw(subTitleTextTex, subTitleTextPos);	
}

void PhotoCardStyle::activateListeners()
{
	sixBtnLayer->connectEventHandler(&PhotoCardStyle::buttonClicked, this);
	Sprite::activateListeners();
}	

void PhotoCardStyle::unActivateListeners()
{
	sixBtnLayer->disconnectEventHandler();
	Sprite::unActivateListeners();
}	

void PhotoCardStyle::buttonClicked(EventGUIRef& event)
{
	EventGUI *ev = event.get();
	if(ev) mouseUpSignal(event);
}