#include "main/designBlock/DesignsLayout.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;

DesignsLayout::DesignsLayout(ConfigSettingsRef configSettings, const ci::Vec2i& position) 
	:Sprite(), configSettings(configSettings)
{
	setPosition(position);

	returnButton = ReturnButtonRef(new ReturnButton(configSettings, ci::Vec2f(0.0f, 891.0f)));
	addChild(returnButton);

	screenSaver = ScreenSaverBlockRef(new ScreenSaverBlock(configSettings, ci::Vec2f(0.0f, 690.0f)));
	addChild(screenSaver);

	designChooser = DesignChooserRef(new DesignChooser(configSettings, ci::Vec2f(0.0f, 2.0f)));
	addChild(designChooser);
}

void DesignsLayout::activateListeners()
{
	designChooser->activateListeners();
	screenSaver->activateListeners();

	designChooser->connectEventHandler(&DesignsLayout::designChanged, this, DesignChooser::CHANGED_DESIGN);
	designChooser->connectEventHandler(&DesignsLayout::openUserDesignFolder, this, DesignChooser::OPEN_USER_DESIGN_FOLDER);
	screenSaver->connectEventHandler(&DesignsLayout::checkerChanged, this, ScreenSaverBlock::SCREEN_SAVER_STATE);
	screenSaver->connectEventHandler(&DesignsLayout::openScreenSaverFolder, this, ScreenSaverBlock::SCREEN_SAVER_OPEN_FOLDER);
	returnButton->connectEventHandler(&DesignsLayout::returnHandler, this);
}

void DesignsLayout::unActivateListeners()
{
	designChooser->unActivateListeners();
	designChooser->disconnectEventHandler(DesignChooser::CHANGED_DESIGN);
	designChooser->disconnectEventHandler(DesignChooser::OPEN_USER_DESIGN_FOLDER);

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

void DesignsLayout::designChanged()
{
	callback(CHANGED_DESIGN);
}

void DesignsLayout::openUserDesignFolder()
{
	callback(OPEN_USER_DESIGN_FOLDER);
}

bool DesignsLayout::getScreenSaverValue() const
{
	return screenSaver->getScreenSaverValue();
}

int DesignsLayout::getDesignID() const
{
	return designChooser->getDesignID();
}