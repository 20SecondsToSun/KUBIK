#include "PhotoInstruction.h"

using namespace std;
using namespace ci::signals;
using namespace kubik::games::photobooth;
using namespace kubik;

PhotoInstruction::PhotoInstruction(PhotoboothSettingsRef settings)
	:animTime(0.8f), alphaAnim(1.0f)
{
	titlePositionY = 492.0f;
	voidBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(1080, 1920, Vec2f(0.0f, 80.0f)));
	voidBtn->setAlpha(1.0f);
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
	console() << "start Instruction" <<endl;	
	voidBtn->connectEventHandler(&PhotoInstruction::hideAnimation, this);
}

void PhotoInstruction::stop()
{
	console() << "stop Instruction" << endl;
	stopAllTweens();
	voidBtn->disconnectEventHandler();
}

void PhotoInstruction::hideAnimation(EventGUIRef& event)
{	
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
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(fonTex);
	gl::draw(titleTex, titleTexPos);
	//voidBtn->draw();
}

void PhotoInstruction::stopAllTweens()
{
	alphaAnim.stop();
	IPhotoboothLocation::stopAllTweens();
}