#include "states/sharing/Popup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik;

Popup::Popup()
{

}

void Popup::show()
{
	timeline().apply(&alphaAnim, 0.0f, 0.8f, 0.8f, EaseOutCubic())
		.finishFn(bind(&Popup::showAnimComplete, this));	
}

void Popup::showAnimComplete()
{
	connectEventHandler(&Popup::hide, this);	
}

void Popup::hideAnimComplete()
{
	callback(POPUP_CLOSED);
}

void Popup::hide(EventGUIRef& event)
{
	disconnectEventHandler();
	timeline().apply(&alphaAnim, 0.0f, 0.8f, EaseOutCubic())
		.finishFn(bind(&Popup::hideAnimComplete, this));
}

void Popup::draw()
{
	auto color = Utils::colorAlpha(Color::hex(0x33373d), alphaAnim);
	gl::color(color);
	gl::drawSolidRect(getWindowBounds());
	gl::color(Color::white());
}