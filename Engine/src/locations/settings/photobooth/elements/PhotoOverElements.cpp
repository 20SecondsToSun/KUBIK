#include "PhotoOverElements.h"

using namespace kubik;
using namespace kubik::config;

PhotoOverElements::PhotoOverElements(PhotoboothSettingsRef settings, const ci::Color& color, int index)
	:IPhotoboothItem(settings, PhtTextID::PHOTO_OVER, color, index)
{
	DesignData designdata = settings->getPhotoOverDesignData();	
	int activeID		  = settings->getActiveOverDesignID();
	int userDesignID	  = settings->getUserOverDesignID();
	std::string syspath	  = settings->getUserPhotoOverDesignPath();

	sixBtnLayer = SixButtonsLayerPhotoOverRef(new SixButtonsLayer<ChangePhotoOverDesignEvent>(designdata, activeID, userDesignID, syspath));
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