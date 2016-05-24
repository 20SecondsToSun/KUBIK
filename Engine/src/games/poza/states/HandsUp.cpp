#include "HandsUp.h"

using namespace std;
using namespace ci;
using namespace ci::signals;
using namespace kubik::games::poza;
using namespace kubik;
using namespace kubik::config;

HandsUp::HandsUp(PozaSettingsRef settings)
	:animTime(0.8f),
	alphaAnim(1.0f)
{
	titlePositionY = 237.0f;
	voidBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(1080, 1920, Vec2f(0.0f, 80.0f)));
	voidBtn->setAlpha(1.0f);
	reset(settings);
}

HandsUp::~HandsUp()
{
	logger().log("~~~ Poza.SubLocation HandsUp.Destruct  ~~~");
}

void HandsUp::reset(ISettingsRef set)
{
	logger().log("~~~ Poza.SubLocation HandsUp.Reset ~~~");

	IGameLocation::reset(set);
	settings = set;

	titlte = settings->getTexture("handsup");
	man = settings->getTexture("man");

	titleTexPos = Vec2f(0.5f * (app::getWindowWidth() - titlte.getWidth()), titlePositionY - titlte.getHeight() * 0.5f);
	manTexPos   = Vec2f(0.5f * (app::getWindowWidth() - man.getWidth()), 464.0f);
}

void HandsUp::start()
{
	logger().log("~~~ Poza.SubLocation HandsUp.Start ~~~");

	voidBtn->connectEventHandler(&HandsUp::handsUpDetectionHandler, this);
	delaycall(bind(&HandsUp::initAnimationcomplete, this), 0.4f);
}

void HandsUp::initAnimationcomplete()
{
	callback(COMPLETE_ANIM);
}

void HandsUp::stop()
{
	logger().log("~~~ Poza.SubLocation HandsUp.Stop ~~~");

	stopAllTweens();
	disconnectAllListeners();	
}

void HandsUp::handsUpDetectionHandler(EventGUIRef& event)
{
	hideAnimation(event);
}

void HandsUp::hideAnimation(EventGUIRef& event)
{
	voidBtn->disconnectEventHandler();
	callback(BEGIN_ANIM);
	app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&HandsUp::hideAnimationComplete, this));
}

void HandsUp::hideAnimationComplete()
{
	alphaAnim = 1.0f;
	IGameLocation::hideAnimationComplete();
}

void HandsUp::update()
{

}

void HandsUp::draw()
{
	fillBg();

	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(titlte, Vec2i(titleTexPos));
	gl::draw(man, Vec2i(manTexPos));
}

void HandsUp::stopAllTweens()
{
	alphaAnim.stop();
	IGameLocation::stopAllTweens();
}

void HandsUp::disconnectAllListeners()
{
	voidBtn->disconnectEventHandler();
}