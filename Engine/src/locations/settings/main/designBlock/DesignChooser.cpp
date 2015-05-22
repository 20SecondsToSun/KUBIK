#include "main/designBlock/DesignChooser.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

DesignChooser::DesignChooser(ConfigSettingsRef configSettings, const ci::Vec2i& position)
	:Sprite(), configSettings(configSettings)
{
	setPosition(position);

	auto designdata   = configSettings->getDesignData();
	auto activeID	  = configSettings->getActiveDesignID();
	auto userDesignID = configSettings->getUserDesignID();
	auto syspath	  = configSettings->getUserDesignPath();
	auto over6		  = configSettings->getTexture("over6");

	SixButtonsInitObject initObj(designdata, activeID, userDesignID, syspath, Color::hex(0x0d0917), over6, 0.0f, 0.0f);
	sixBtnLayer = SixButtonsLayerDesignRef(new SixButtonsLayer<ChangeDesignEvent>(initObj));
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