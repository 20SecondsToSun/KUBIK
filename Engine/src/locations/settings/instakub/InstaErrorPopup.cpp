#include "InstaErrorPopup.h"

using namespace kubik;
using namespace kubik::config;

InstaErrorPopup::InstaErrorPopup(const gl::Texture& image)
	:alphaColor(0.0f),
	showing(false),
	image(image)
{
	btn = SimpleSpriteButtonRef(new SimpleSpriteButton(Rectf(0.0f, 0.0f, 1080.0f, 1920.f)));
	btn->setAlpha(0.0f);
	addChild(btn);
}

void InstaErrorPopup::hide(const EaseFn& eFunc, float time)
{
	showing = false;
	timeline().apply(&alphaColor, 0.0f, time, eFunc);
	btn->disconnectEventHandler();
}

void InstaErrorPopup::show(const EaseFn& eFunc, float time)
{
	if (!showing)
	{
		showing = true;
		timeline().apply(&alphaColor, 0.0f, 0.97f, time, eFunc);
		btn->connectEventHandler(&InstaErrorPopup::mouseUpHandler, this);
	}
}

void InstaErrorPopup::mouseUpHandler(EventGUIRef& event)
{
	callback(HIDE_POPUP);
}

void InstaErrorPopup::close()
{
	showing = false;
	alphaColor = 0.0f;
	btn->disconnectEventHandler();
}

void InstaErrorPopup::drawLayout()
{
	btn->setAlpha(0.0f);
	gl::color(Utils::colorAlpha(Color::hex(0x313a43), alphaColor));	
	gl::drawSolidRect(Rectf(Vec2f::zero(), Vec2f(1080.0f, 1920.0f)));
	gl::color(ColorA(1, 1, 1, alphaColor));
	gl::draw(image, Vec2f(125.0f, 575.0f));
	gl::color(Color::white());
}