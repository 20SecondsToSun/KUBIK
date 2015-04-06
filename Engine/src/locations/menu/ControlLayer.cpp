#include "ControlLayer.h"

using namespace kubik::menu;
using namespace kubik;
using namespace ci;
using namespace std;

ControlLayer::ControlLayer(ISettingsRef settings)
	:tex(settings->getTexture("menuButton"))
{
	btn = ImageButtonSpriteRef(new ImageButtonSprite(tex, Vec2f(44.0f, 44.0f)));
	createControlsButtons();
}

void ControlLayer::showBackButton()
{
	addChild(btn);
}

void ControlLayer::activateListeners()
{
	btn->connectEventHandler(&ControlLayer::close, this);
	settingsbtn->connectEventHandler(&ControlLayer::startSettings, this);
	btn->setAlpha(1.0f);
	settingsbtn->setAlpha(1.0f);
}

void ControlLayer::hideBackButton()
{
	removeChild(btn);
}

void ControlLayer::close(EventGUIRef& event)
{
	callback(CLOSE_GAME);
}

void ControlLayer::unActivateListeners()
{
	btn->setAlpha(0.2);
	settingsbtn->setAlpha(0.2);
	btn->disconnectEventHandler();
	settingsbtn->disconnectEventHandler();	
}

void ControlLayer::hideButtons()
{
	btn->setAlpha(0);
	settingsbtn->setAlpha(0);
}

void ControlLayer::draw()
{
	Sprite::draw();
}

void ControlLayer::createControlsButtons()
{
	Rectf setButtonArea = Rectf(850.0f, 0.0f, 1050.0f, 15.0f);
	settingsbtn = SimpleSpriteButtonRef(new SimpleSpriteButton(setButtonArea));
	settingsbtn->connectEventHandler(&ControlLayer::startSettings, this);
	addChild(settingsbtn);
}

void ControlLayer::startSettings(EventGUIRef& event)
{
	callback(OPEN_SETTINGS);
}

void ControlLayer::clear()
{
	settingsbtn->disconnectEventHandler();
	btn->disconnectEventHandler();
	removeChild(settingsbtn);
	removeChild(btn);
}