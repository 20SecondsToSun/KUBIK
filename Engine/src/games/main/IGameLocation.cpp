#include "IGameLocation.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games;
using namespace ci;

ISettingsRef IGameLocation::settings;

IGameLocation::IGameLocation() 
	:titlePositionY(238.0f),
	animShowTitleTime(1.1f)
{

}

void IGameLocation::nextLocationSignal(EventGUIRef& event)
{
	stop();
	callback(NEXT_LOC);
}

void IGameLocation::hideAnimationComplete()
{
	stop();
	callback(NEXT_LOC);
}

void IGameLocation::reset(ISettingsRef settings)
{
	this->settings = settings;
	bckgrnd = ci::gl::TextureRef(new ci::gl::Texture(settings->getTexture("bg")));
}

void IGameLocation::fillBg()
{
	if (bckgrnd)
	{
		gl::draw(bckgrnd);
	}
}

void IGameLocation::drawTitle()
{
	gl::color(ColorA(1.0f, 1.0f, 1.0f, titleAlpha));
	gl::draw(title, titleAnimPosition);
	gl::color(Color::white());
}

void IGameLocation::setLastScreenShot()
{
	screenshot = Utils::drawGraphicsToFBO(app::getWindowSize(), [&](){ draw(); });
}

void IGameLocation::stopAllTweens()
{
	titleAlpha.stop();
	titleScale.stop();
	titleFilterAlpha.stop();
	titleAnimPosition.stop();
}

void IGameLocation::setDbRecord(std::shared_ptr<DataBaseRecord> db)
{
	dbRecord = db;
}