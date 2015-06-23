#include "IPhotoboothLocation.h"

using namespace kubik;
using namespace kubik::games::photobooth;

PhotoboothSettingsRef IPhotoboothLocation::settings;

IPhotoboothLocation::IPhotoboothLocation():titlePositionY(238.0f), animShowTitleTime(1.1f)
{

}

void IPhotoboothLocation::nextLocationSignal(EventGUIRef& event)
{ 
	stop();
	callback(NEXT_LOC);
};

void IPhotoboothLocation::hideAnimationComplete()
{ 
	stop();
	callback(NEXT_LOC);
};

void IPhotoboothLocation::reset(PhotoboothSettingsRef settings)
{
	this->settings = settings;
	bckgrnd  = TextureRef(new Texture(settings->getTexture("bg")));
}

void IPhotoboothLocation::fillBg()
{
	if (bckgrnd)
		gl::draw(bckgrnd);
}

void IPhotoboothLocation::drawTitle()
{
	gl::color(ColorA(1.0f, 1.0f, 1.0f, titleAlpha));
	gl::draw(title, titleAnimPosition);
	gl::color(Color::white());
}

void IPhotoboothLocation::setLastScreenShot()
{
	screenshot = Utils::drawGraphicsToFBO(getWindowSize(), [&](){ draw(); });
}

void IPhotoboothLocation::stopAllTweens()
{
	titleAlpha.stop();
	titleScale.stop();
	titleFilterAlpha.stop();
	titleAnimPosition.stop();
}