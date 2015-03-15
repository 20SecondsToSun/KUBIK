#include "IPhotoboothLocation.h"

using namespace kubik;
using namespace kubik::games::photobooth;

PhotoboothSettingsRef IPhotoboothLocation::settings;
ci::gl::Texture IPhotoboothLocation::bckgrnd;

IPhotoboothLocation::IPhotoboothLocation():titlePositionY(238.0f), animShowTitleTime(0.6f)
{

}

void IPhotoboothLocation::nextLocationSignal(EventGUIRef& event)
{ 
	callback(NEXT_LOC);
};

void IPhotoboothLocation::hideAnimationComplete()
{ 
	callback(NEXT_LOC);
};

void IPhotoboothLocation::reset(PhotoboothSettingsRef settings)
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