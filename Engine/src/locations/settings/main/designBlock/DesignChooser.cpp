#include "main/designBlock/DesignChooser.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

DesignChooser::DesignChooser(ConfigSettingsRef configSettings, const ci::Vec2i& position)
	:Sprite(), configSettings(configSettings)
{
	setPosition(position);

	DesignData designdata = configSettings->getDesignData();
	int activeID		  = configSettings->getActiveDesignID();
	int userDesignID	  = configSettings->getUserDesignID();
	std::string syspath   = configSettings->getUserDesignPath();

	sixBtnLayer = SixButtonsLayerDesignRef(new SixButtonsLayer<ChangeDesignEvent>(designdata, activeID, userDesignID, syspath, 0.0f, 0.0f));
	addChild(sixBtnLayer);
}

void DesignChooser::activateListeners()
{
	sixBtnLayer->connectEventHandler(&DesignChooser::buttonClicked, this);
	Sprite::activateListeners();
}

void DesignChooser::unActivateListeners()
{
	sixBtnLayer->disconnectEventHandler();
	Sprite::unActivateListeners();
}

void DesignChooser::buttonClicked(EventGUIRef& event)
{
	EventGUI *ev = event.get();
	if (ev) mouseUpSignal(event);
}