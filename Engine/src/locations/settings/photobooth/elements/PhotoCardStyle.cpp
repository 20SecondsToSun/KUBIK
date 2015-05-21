#include "PhotoCardStyle.h"

using namespace kubik;
using namespace kubik::config;

PhotoCardStyle::PhotoCardStyle(PhotoboothSettingsRef phbSettings, const ci::Color& color, int index)
	:IPhotoboothItem(phbSettings, PhtTextID::CARD_STYLE, color, index)
{
	auto designdata = settings->getPhotoCardStyles();
	auto activeID = settings->getActivePhotoCardStyleDesignID();
	auto userDesignID = settings->getUserPhotoCardStyleDesignID();
	auto syspath = settings->getUserPhotoCardStylePath();
	auto over6 = settings->getTexture("over6");

	sixBtnLayer = SixButtonsLayerPhotoCardRef(new SixButtonsLayer<ChangePhotoCardStyleDesignEvent>(designdata, activeID, userDesignID, syspath, over6));
	addChild(sixBtnLayer);			
}

void PhotoCardStyle::activateListeners()
{
	sixBtnLayer->connectEventHandler(&PhotoCardStyle::buttonClicked, this);
	IPhotoboothItem::activateListeners();
}

void PhotoCardStyle::unActivateListeners()
{
	sixBtnLayer->disconnectEventHandler();
	IPhotoboothItem::unActivateListeners();
}		

void PhotoCardStyle::buttonClicked(EventGUIRef& event)
{
	EventGUI *ev = event.get();
	if(ev) mouseUpSignal(event);
}