#include "PozaInstruction.h"

using namespace std;
using namespace ci;
using namespace ci::signals;
using namespace kubik::games::poza;
using namespace kubik;
using namespace kubik::config;

PozaInstruction::PozaInstruction(PozaSettingsRef settings)
	:animTime(0.8f),
	alphaAnim(1.0f)
{
	titlePositionY = 492.0f;
	voidBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(1080, 1920, Vec2f(0.0f, 80.0f)));
	voidBtn->setAlpha(1.0f);
	reset(settings);
}

PozaInstruction::~PozaInstruction()
{
	logger().log("~~~ Poza.SubLocation PozaInstruction.Destruct ~~~");
}

void PozaInstruction::reset(ISettingsRef set)
{
	logger().log("~~~ Poza.SubLocation PozaInstruction.Reset ~~~");

	IGameLocation::reset(set);
	settings = set;
	fonTex = settings->getTexture("instrFon");
}

void PozaInstruction::start()
{
	logger().log("~~~ Poza.SubLocation PozaInstruction.Start ~~~");

	voidBtn->connectEventHandler(&PozaInstruction::hideAnimation, this);
	delaycall(bind(&PozaInstruction::initAnimationcomplete, this), 0.4f);
}

void PozaInstruction::initAnimationcomplete()
{
	callback(COMPLETE_ANIM);
}

void PozaInstruction::stop()
{
	logger().log("~~~ Poza.SubLocation PozaInstruction.Stop ~~~");

	stopAllTweens();
	voidBtn->disconnectEventHandler();
}

void PozaInstruction::hideAnimation(EventGUIRef& event)
{
	callback(BEGIN_ANIM);
	app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&PozaInstruction::hideAnimationComplete, this));
}

void PozaInstruction::hideAnimationComplete()
{
	alphaAnim = 1.0f;
	IGameLocation::hideAnimationComplete();
}

void PozaInstruction::update()
{

}

void PozaInstruction::draw()
{
	fillBg();
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(fonTex);
}

void PozaInstruction::stopAllTweens()
{
	alphaAnim.stop();
	IGameLocation::stopAllTweens();
}