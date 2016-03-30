#include "PhotoInstruction.h"

using namespace std;
using namespace ci::signals;
using namespace kubik::games::photobooth;
using namespace kubik;

PhotoInstruction::PhotoInstruction(PhotoboothSettingsRef settings):animTime(0.8f), alphaAnim(1.0f)
{
	titlePositionY = 492.0f;
	voidBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(1080, 1920, Vec2f(0.0f, 80.0f)));
	voidBtn->setAlpha(1.0f);
	reset(settings);
};

PhotoInstruction::~PhotoInstruction()
{
	logger().log("~~~ Photobooth.SubLocation PhotoInstruction.Destruct  ~~~");
}

void PhotoInstruction::reset(PhotoboothSettingsRef set)
{
	logger().log("~~~ Photobooth.SubLocation PhotoInstruction.Reset ~~~");

	IPhotoboothLocation::reset(set);
	settings	= set;
	fonTex		= settings->getTexture("instrFon");
	titleTex	= settings->getTexture("instrTitle");
	titleTexPos = Vec2f(0.5f * (getWindowWidth() - titleTex.getWidth()), titlePositionY - titleTex.getHeight() * 0.5f);
}

void PhotoInstruction::start()
{
	logger().log("~~~ Photobooth.SubLocation PhotoInstruction.Start ~~~");

	voidBtn->connectEventHandler(&PhotoInstruction::hideAnimation, this);
	delaycall(bind(&PhotoInstruction::initAnimationcomplete, this), 0.4f);
}

void PhotoInstruction::initAnimationcomplete()
{
	callback(COMPLETE_ANIM);
}

void PhotoInstruction::stop()
{
	logger().log("~~~ Photobooth.SubLocation PhotoInstruction.Stop ~~~");

	stopAllTweens();
	voidBtn->disconnectEventHandler();
}

void PhotoInstruction::hideAnimation(EventGUIRef& event)
{	
	voidBtn->disconnectEventHandler();
	callback(BEGIN_ANIM);
	timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&PhotoInstruction::hideAnimationComplete, this));
}

void PhotoInstruction::hideAnimationComplete()
{
	alphaAnim = 1.0f;
	IPhotoboothLocation::hideAnimationComplete();	
}

void PhotoInstruction::update()
{

}

void PhotoInstruction::draw()
{
	fillBg();

	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(fonTex);
	gl::draw(titleTex, Vec2i(titleTexPos));
}

void PhotoInstruction::stopAllTweens()
{
	alphaAnim.stop();
	IPhotoboothLocation::stopAllTweens();
}