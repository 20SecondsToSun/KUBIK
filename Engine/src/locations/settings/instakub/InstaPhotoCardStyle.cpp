#include "InstaPhotoCardStyle.h"

using namespace kubik::config;
using namespace kubik;
using namespace ci;

InstaPhotoCardStyle::InstaPhotoCardStyle(InstakubSettingsRef settings, const ci::Vec2i& position):Sprite()
{
	setPosition(position);	

	titleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::PHOTO_TITLE_MAIN));
	titleTextPos = Vec2f(0.5f * (914.0f - titleTextTex.getWidth()), 100.0f);			

	subTitleTextTex = textTools().getTextField(settings->getTextItem(InstakubSettings::InstaTextID::PHOTO_TITLE_SUB));
	subTitleTextPos = Vec2f(0.5f * (914.0f - subTitleTextTex.getWidth()), 166.0f);	

	DesignData designdata = settings->getPhotoCardStyles();				
	int activeID		  = settings->getActivePhotoCardStyleDesignID();
	int userDesignID	  = settings->getUserPhotoCardStyleDesignID();
	std::string syspath   = settings->getUserPhotoCardStylePath();

	sixBtnLayer = SixButtonsLayerInstaRef(new SixButtonsLayer<ChangePhotoCardStyleDesignEvent>(designdata, activeID, userDesignID, syspath));
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
	gl::color(Color::hex(0x578d24));
	gl::drawSolidRect(Rectf(Vec2f::zero(), Vec2f(914.0f, 1000.0f)));
	gl::color(Color::white());
}

void InstaPhotoCardStyle::drawTitles()
{
	gl::draw(titleTextTex, titleTextPos);
	gl::draw(subTitleTextTex, subTitleTextPos);	
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