#include "KeyBackground.h"

using namespace kubik;
using namespace kubik::config;

KeyBackground::KeyBackground(const Vec2f& initPosition)
	:alphaColorPlashka(0.0f),
	showing(false),
	initPosition(initPosition),
	plashkaHeight(592.0f),
	bgHeight(1920.0f - 592.0f - initPosition.x)
{
	btn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(0.0f,0.0f, 1080.0f, 80.f)));
	btn->setPosition(initPosition);
	addChild(btn);
}

KeyBackground::KeyBackground(const Vec2f& initPosition, const ci::gl::Texture& closeKeyboard)
	:alphaColorPlashka(0.0f),
	showing(false),
	initPosition(initPosition),
	plashkaHeight(592.0f),
	bgHeight(1920.0f - 592.0f - initPosition.x)
{	
	this->closeKeyboard = closeKeyboard;

	auto positionY = /* 885.0f*/ 600.0f - closeKeyboard.getHeight() * 0.5f;
	auto positionX = (1080.0f - closeKeyboard.getWidth()) * 0.5f;

	btn = ImageButtonSpriteRef(new ImageButtonSprite(closeKeyboard));
	btn->setPosition(initPosition + Vec2f(positionX, positionY));
	addChild(btn);
}

void KeyBackground::hide(const EaseFn& eFunc, float time)
{	
	showing = false;
	timeline().apply(&alphaColorPlashka, 0.0f, time, eFunc);
	timeline().apply(&alphaColorBg, 0.0f, time, eFunc);
	timeline().apply(&animPositionY, 0.0f, 1920.0f - initPosition.y, time, eFunc);
	btn->disconnectEventHandler();
}

void KeyBackground::show(const EaseFn& eFunc, float time)
{
	if (!showing)
	{
		showing = true;
		timeline().apply(&animPositionY, 1920.0f - initPosition.y, 0.0f, time, eFunc);
		timeline().apply(&alphaColorPlashka, 0.0f, 0.97f, time, eFunc);
		timeline().apply(&alphaColorBg, 0.0f, 0.86f, time, eFunc);
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
	alphaColorPlashka = 0.0f;
	alphaColorBg = 0.0f;
	btn->disconnectEventHandler();
}

void KeyBackground::drawLayout()
{
	gl::translate(0.0f, animPositionY);
	btn->setAlpha(alphaColorPlashka);
	gl::color(Utils::colorAlpha(Color::hex(0x313a43), alphaColorPlashka));
	gl::drawSolidRect(Rectf(initPosition, Vec2f(initPosition.x + 1080.0f, initPosition.y + plashkaHeight)));

	gl::color(Utils::colorAlpha(Color::hex(0x131417), alphaColorBg));
	gl::drawSolidRect(Rectf(initPosition + Vec2f(0.0f, plashkaHeight), Vec2f(initPosition.x + 1080.0f, bgHeight)));
	gl::color(Color::white());
}