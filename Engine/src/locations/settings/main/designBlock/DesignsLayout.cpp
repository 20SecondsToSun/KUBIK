#include "main/designBlock/DesignsLayout.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

DesignsLayout::DesignsLayout(ConfigSettingsRef configSettings, ScreenSaverSettingsRef ssSettings, const ci::Vec2i& position)
	:Sprite(), configSettings(configSettings)
{
	setPosition(position);

	returnButton = ReturnButtonRef(new ReturnButton(configSettings, Vec2f(0.0f, 891.0f)));
	addChild(returnButton);

	screenSaver = ScreenSaverBlockRef(new ScreenSaverBlock(configSettings, ssSettings, Vec2f(0.0f, 690.0f)));
	addChild(screenSaver);

	designChooser = DesignChooserRef(new DesignChooser(configSettings, Vec2f(0.0f, 2.0f)));
	addChild(designChooser);
}

void DesignsLayout::activateListeners()
{
	designChooser->activateListeners();
	designChooser->connectEventHandler(&DesignsLayout::callbackHandler, this);

	screenSaver->activateListeners();
	screenSaver->connectEventHandler(&DesignsLayout::checkerChanged, this, ScreenSaverBlock::SCREEN_SAVER_STATE);
	screenSaver->connectEventHandler(&DesignsLayout::openScreenSaverFolder, this, ScreenSaverBlock::SCREEN_SAVER_OPEN_FOLDER);
	returnButton->connectEventHandler(&DesignsLayout::returnHandler, this);
}

void DesignsLayout::callbackHandler(EventGUIRef& event)
{
	EventGUI *ev = event.get();
	if (ev) mouseUpSignal(event);
}

void DesignsLayout::unActivateListeners()
{
	designChooser->unActivateListeners();
	designChooser->disconnectEventHandler();
	
	screenSaver->unActivateListeners();
	screenSaver->disconnectEventHandler(ScreenSaverBlock::SCREEN_SAVER_STATE);
	screenSaver->disconnectEventHandler(ScreenSaverBlock::SCREEN_SAVER_OPEN_FOLDER);

	returnButton->disconnectEventHandler();
}

void DesignsLayout::checkerChanged()
{
	callback(SCREEN_SAVER_STATE);
}

void DesignsLayout::openScreenSaverFolder()
{
	callback(SCREEN_SAVER_OPEN_FOLDER);
}

void DesignsLayout::returnHandler(EventGUIRef& event)
{
	callback(HIDE);
}

bool DesignsLayout::getScreenSaverValue() const
{
	return screenSaver->getScreenSaverValue();
}