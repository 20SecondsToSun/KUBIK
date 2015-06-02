#include "InstaPhotoCardStyle.h"

using namespace kubik::config;
using namespace kubik;
using namespace ci;

InstaPhotoCardStyle::InstaPhotoCardStyle(InstakubSettingsRef settings, const ci::Vec2i& position)
	:Sprite(),
	backgroundColor(Color::hex(0x578d24)),
	titleTextPos(Vec2f(0.0f, 100.0f)),
	subTitleTextPos(Vec2f(0.0f, 145.0f))
{
	setPosition(position);	

	titleItem = settings->getTextItem(InstakubSettings::InstaTextID::PHOTO_TITLE_MAIN);
	subTitleItem = settings->getTextItem(InstakubSettings::InstaTextID::PHOTO_TITLE_SUB);

	auto designdata   = settings->getPhotoCardStyles();
	auto activeID	  = settings->getActivePhotoCardStyleDesignID();
	auto userDesignID = settings->getUserPhotoCardStyleDesignID();
	auto syspath	  = settings->getUserPhotoCardStylePath();
	auto over6		  = settings->getTexture("over6");

	SixButtonsInitObject initObj(designdata, activeID, userDesignID, syspath, Color::hex(0x578d24), over6, 106.0f, 278.0f);
	sixBtnLayer = SixButtonsLayerInstaRef(new SixButtonsLayer<ChangePhotoCardStyleDesignEvent>(initObj));
	addChild(sixBtnLayer);	
}

void InstaPhotoCardStyle::drawLayout()
{
	drawBackground();
	drawTitles();
	Sprite::drawLayout();
}	

void InstaPhotoCardStyle::drawBackground()
{
	gl::color(backgroundColor);
	gl::drawSolidRect(Rectf(Vec2f::zero(), Vec2f(914.0f, 1000.0f)));
	gl::color(Color::white());
}

void InstaPhotoCardStyle::drawTitles()
{
	textTools().drawTextBox(titleItem, backgroundColor, titleTextPos, Vec2i(914.0f, 50.0f));
	textTools().drawTextBox(subTitleItem, backgroundColor, subTitleTextPos, Vec2i(914.0f, 50.0f));
}

void InstaPhotoCardStyle::activateListeners()
{
	sixBtnLayer->connectEventHandler(&InstaPhotoCardStyle::buttonClicked, this);
	Sprite::activateListeners();
}	

void InstaPhotoCardStyle::unActivateListeners()
{
	sixBtnLayer->disconnectEventHandler();
	Sprite::unActivateListeners();
}	

void InstaPhotoCardStyle::buttonClicked(EventGUIRef& event)
{
	EventGUI *ev = event.get();
	if(ev) mouseUpSignal(event);
}