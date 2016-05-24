#include "ThreeMeters.h"

using namespace std;
using namespace ci;
using namespace ci::signals;
using namespace kubik::games::poza;
using namespace kubik;
using namespace kubik::config;

ThreeMeters::ThreeMeters(PozaSettingsRef settings) 
	:animTime(0.8f), 
	alphaAnim(1.0f)
{
	titlePositionY = 237.0f;
	voidBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(1080, 1920, Vec2f(0.0f, 80.0f)));
	voidBtn->setAlpha(1.0f);
	reset(settings);
}

ThreeMeters::~ThreeMeters()
{
	logger().log("~~~ Poza.SubLocation ThreeMeters.Destruct  ~~~");
}

void ThreeMeters::reset(ISettingsRef set)
{
	logger().log("~~~ Poza.SubLocation ThreeMeters.Reset ~~~");

	IGameLocation::reset(set);
	settings = set;

	thrimeters = settings->getTexture("3meters");
	arrow = settings->getTexture("arrow");

	titleTexPos = Vec2f(0.5f * (app::getWindowWidth() - thrimeters.getWidth()), titlePositionY	   - thrimeters.getHeight() * 0.5f);
	arrowTexPos = Vec2f(0.5f * (app::getWindowWidth() - arrow.getWidth()), 590);// (app::getWindowHeight() - arrow.getHeight()) * 0.5f);
}

void ThreeMeters::start()
{
	logger().log("~~~ Poza.SubLocation ThreeMeters.Start ~~~");

	voidBtn->connectEventHandler(&ThreeMeters::hideAnimation, this);
	delaycall(bind(&ThreeMeters::initAnimationcomplete, this), 0.4f);
}

void ThreeMeters::initAnimationcomplete()
{
	callback(COMPLETE_ANIM);
}

void ThreeMeters::stop()
{
	logger().log("~~~ Poza.SubLocation ThreeMeters.Stop ~~~");

	stopAllTweens();
	voidBtn->disconnectEventHandler();
}

void ThreeMeters::hideAnimation(EventGUIRef& event)
{
	callback(BEGIN_ANIM);
	app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&ThreeMeters::hideAnimationComplete, this));
}

void ThreeMeters::hideAnimationComplete()
{
	alphaAnim = 1.0f;
	IGameLocation::hideAnimationComplete();
}

void ThreeMeters::update()
{

}

void ThreeMeters::draw()
{
	fillBg();
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(thrimeters, Vec2i(titleTexPos));
	gl::draw(arrow, Vec2i(arrowTexPos));
}

void ThreeMeters::stopAllTweens()
{
	alphaAnim.stop();
	IGameLocation::stopAllTweens();
}