#include "IPhotoboothLocation.h"

using namespace kubik;

PhotoboothSettingsRef IPhotoboothLocation::settings;
ci::gl::Texture IPhotoboothLocation::bckgrnd;

void IPhotoboothLocation::nextLocationSignal(EventGUIRef& event)
{ 
	callback(NEXT_LOC);
};

void IPhotoboothLocation::reset(PhotoboothSettingsRef config)
{
	this->settings = settings;
	bckgrnd  = settings->getTexture("bg");
}

void IPhotoboothLocation::fillBg()
{
	//gl::color(ci::Color::hex(0x060a0e));
	//gl::drawSolidRect(getWindowBounds());
	//gl::color(ci::Color::white());
	gl::draw(bckgrnd);
}