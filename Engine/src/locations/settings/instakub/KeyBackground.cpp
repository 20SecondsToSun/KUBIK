#include "KeyBackground.h"

using namespace kubik;
using namespace kubik::config;

KeyBackground::KeyBackground(const Vec2f& initPosition)
	:alphaColor(0.0f),
	showing(false),
	initPosition(initPosition)
{
	btn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(0.0f,0.0f, 1080.0f, 80.f)));
	btn->setPosition(initPosition);
	addChild(btn);
}

void KeyBackground::hide(const EaseFn& eFunc, float time)
{	
	showing = false;
	timeline().apply(&alphaColor, 0.0f, time, eFunc);
	btn->disconnectEventHandler();
}

void KeyBackground::show(const EaseFn& eFunc, float time)
{
	if (!showing)
	{
		showing = true;
		timeline().apply(&alphaColor, 0.0f, 0.97f, time, eFunc);
		btn->connectEventHandler(&KeyBackground::mouseUpHandler, this);
	}	
}

void KeyBackground::mouseUpHandler(EventGUIRef& event)
{
	callback(HIDE_KEYBOARD);
}

void KeyBackground::close()
{
	showing = false;
	alphaColor = 0.0f;
	btn->disconnectEventHandler();
}

void KeyBackground::drawLayout()
{
	btn->setAlpha(alphaColor);
	gl::color(Utils::colorAlpha(Color::hex(0x313a43), alphaColor));	
	gl::drawSolidRect(Rectf(initPosition, Vec2f(initPosition.x + 1080, 1920.0f)));
	gl::color(Color::white());
}