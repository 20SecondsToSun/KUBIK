#include "IPhotoboothLocation.h"

using namespace kubik;

PhotoboothSettingsRef IPhotoboothLocation::settings;

void IPhotoboothLocation::nextLocationSignal(EventGUIRef& event)
{ 
	callback(NEXT_LOC);
};

void IPhotoboothLocation::fillBg()
{
	gl::color(ci::Color::hex(0x060a0e));
	gl::drawSolidRect(getWindowBounds());
	gl::color(ci::Color::white());
}