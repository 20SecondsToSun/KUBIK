#include "PhotoOverElements.h"

using namespace kubik;
using namespace kubik::config;

PhotoOverElements::PhotoOverElements(PhotoboothSettingsRef settings, const ci::Color& color, int index)
	:IPhotoboothItem(settings, PhtTextID::PHOTO_OVER, color, index)
{
	auto designdata = settings->getPhotoOverDesignData();
	auto activeID = settings->getActiveOverDesignID();
	auto userDesignID = settings->getUserOverDesignID();
	auto syspath = settings->getUserPhotoOverDesignPath();
	auto over6 = settings->getTexture("over6");

	sixBtnLayer = SixButtonsLayerPhotoOverRef(new SixButtonsLayer<ChangePhotoOverDesignEvent>(designdata, activeID, userDesignID, syspath, over6));
	addChild(sixBtnLayer);	
}

void PhotoOverElements::activateListeners()
{
	sixBtnLayer->connectEventHandler(&PhotoOverElements::buttonClicked, this);
	IPhotoboothItem::activateListeners();
}

void PhotoOverElements::unActivateListeners()
{
	sixBtnLayer->disconnectEventHandler();
	IPhotoboothItem::unActivateListeners();
}		

void PhotoOverElements::buttonClicked(EventGUIRef& event)
{
	EventGUI *ev = event.get();
	if(ev) mouseUpSignal(event);
}