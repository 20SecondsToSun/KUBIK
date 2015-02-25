#include "PhotoCardStyle.h"

using namespace kubik;
using namespace kubik::config;

PhotoCardStyle::PhotoCardStyle(PhotoboothSettingsRef phbSettings, const ci::Color& color, int index)
	:IPhotoboothItem(phbSettings, PhtTextID::CARD_STYLE, color, index)
{
	DesignData designdata = settings->getPhotoCardStyles();	
	int activeID		  = settings->getActivePhotoCardStyleDesignID();
	int userDesignID	  = settings->getUserPhotoCardStyleDesignID();
	std::string syspath	  = settings->getUserPhotoCardStylePath();
	
	sixBtnLayer = SixButtonsLayerPhotoCardRef( new SixButtonsLayer<ChangePhotoCardStyleDesignEvent>(settings, designdata, activeID, userDesignID, syspath));
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