#include "PhotoInstruction.h"

using namespace std;
using namespace ci::signals;
using namespace kubik::games::photobooth;
using namespace kubik;

PhotoInstruction::PhotoInstruction(PhotoboothSettingsRef settings)
	:animTime(0.8f)
{
	titlePositionY = 492.0f;
	reset(settings);
};

void PhotoInstruction::reset(PhotoboothSettingsRef set)
{
	settings = set;
	fonTex = settings->getTexture("instrFon");
	titleTex = settings->getTexture("instrTitle");
	titleTexPos = Vec2f(0.5f * (getWindowWidth() - titleTex.getWidth()), titlePositionY - titleTex.getHeight() * 0.5f);
}

void PhotoInstruction::start()
{
	console() << "start Instruction" << endl;
	alphaAnim = 1.0f;
	connectEventHandler(&PhotoInstruction::hideAnimation, this);
}

void PhotoInstruction::stop()
{
	console() << "stop Instruction" << endl;
	stopAllTweens();
	disconnectEventHandler();
}

void PhotoInstruction::hideAnimation(EventGUIRef& event)
{
	timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&IPhotoboothLocation::hideAnimationComplete, this));
}

void PhotoInstruction::update()
{

}

void PhotoInstruction::draw()
{
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(fonTex);
	gl::draw(titleTex, titleTexPos);
}


void PhotoInstruction::stopAllTweens()
{
	alphaAnim.stop();
	IPhotoboothLocation::stopAllTweens();
}