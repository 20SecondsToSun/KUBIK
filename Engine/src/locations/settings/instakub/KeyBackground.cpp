#include "KeyBackground.h"

using namespace kubik;
using namespace kubik::config;

KeyBackground::KeyBackground()
	:alphaColor(0.0f),
	showing(false)
{
	btn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(0.0f,0.0f, 1080.0f, 80.f)));
	btn->setPosition(Vec2f(-166.0f, 918.0f));
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
	gl::drawSolidRect(Rectf(Vec2f(-166.0f, 918.0f), Vec2f(-166.0f + 1080, 1920.0f)));
	gl::color(Color::white());
}