#include "KeyBackground.h"

using namespace ci;
using namespace ci::app;
using namespace kubik;
using namespace kubik::config;

KeyBackground::KeyBackground(const Vec2f& initPosition)
	:alphaColorPlashka(0.0f),
	showing(false),
	initPosition(initPosition),
	plashkaHeight(592.0f),
	bgHeight(1920.0f - 592.0f - initPosition.x),
	mainColor(Color::hex(0x313a43))
{
	btn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(0.0f,0.0f, 1080.0f, 80.f)));
	btn->setPosition(initPosition);
	addChild(btn);
}

KeyBackground::KeyBackground(const Vec2f& initPosition, const ci::gl::Texture& closeKeyboard, float closeBtnY)
	:alphaColorPlashka(0.0f),
	showing(false),
	initPosition(initPosition),
	plashkaHeight(592.0f),
	bgHeight(1920.0f - 592.0f - initPosition.x),
	closeKeyboard(closeKeyboard),
	mainColor(Color::hex(0x313a43))
{	

#ifdef PORTRAIT_RES
	auto positionY = closeBtnY - closeKeyboard.getHeight() * 0.5f;
#else
	auto positionY =  600.0f - closeKeyboard.getHeight() * 0.5f;
#endif

	auto positionX = (1080.0f - closeKeyboard.getWidth()) * 0.5f;

	btn = ImageButtonSpriteRef(new ImageButtonSprite(closeKeyboard));
	btn->setPosition(initPosition + Vec2f(positionX, positionY));
	addChild(btn);
}

KeyBackground::KeyBackground(const Vec2f& initPosition, const ci::gl::Texture& closeKeyboard, const ci::ColorA& color)	
	:alphaColorPlashka(0.0f),
	showing(false),
	initPosition(initPosition),
	plashkaHeight(592.0f),
	bgHeight(1920.0f - 592.0f - initPosition.x),
	closeKeyboard(closeKeyboard)
{
#ifdef PORTRAIT_RES
	auto positionY = 885.0f - closeKeyboard.getHeight() * 0.5f;
#else
	auto positionY = 600.0f - closeKeyboard.getHeight() * 0.5f;
#endif

	auto positionX = (1080.0f - closeKeyboard.getWidth()) * 0.5f;

	btn = ImageButtonSpriteRef(new ImageButtonSprite(closeKeyboard));
	btn->setPosition(initPosition + Vec2f(positionX, positionY));
	addChild(btn);
	mainColor = color;
}

void KeyBackground::hide(const EaseFn& eFunc, float time)
{	
	showing = false;
	timeline().apply(&alphaColorBtn, 0.0f, 0.2f, eFunc);
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
		timeline().apply(&alphaColorBtn, 0.0f, 1.0f, time, eFunc);
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
	alphaColorBtn = 0.0f;
	alphaColorBg = 0.0f;
	btn->disconnectEventHandler();
}

void KeyBackground::drawLayout()
{
	gl::translate(0.0f, animPositionY);
	btn->setAlpha(alphaColorBtn);
	gl::color(Utils::colorAlpha(mainColor, alphaColorPlashka));
	gl::drawSolidRect(Rectf(initPosition, Vec2f(initPosition.x + 1080.0f, initPosition.y + plashkaHeight)));

	gl::color(Utils::colorAlpha(Color::hex(0x131417), alphaColorBg));
	auto startVec = initPosition + Vec2f(0.0f, plashkaHeight);
	gl::drawSolidRect(Rectf(startVec, startVec + Vec2f(1080.0f, bgHeight)));
	gl::color(Color::white());
}