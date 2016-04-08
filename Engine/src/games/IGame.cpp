#include "IGame.h"

using namespace kubik;
using namespace ci;
using namespace ci::app;

IGame::~IGame()
{

}

void IGame::draw()
{

}

void IGame::clean()
{

}

void IGame::initShowAnimation()
{
	screenshot = getScreenShot();
	state = SHOW_ANIM;
	timeline().apply(&animX, 1080.0f, 0.0f, 0.9f,  EaseOutCubic()).finishFn(std::bind(&IGame::showAnimationComplete, this));
	timeline().apply(&animX1, 0.0f, -500.0f, 0.9f, EaseOutCubic());
	timeline().apply(&alpha, 1.0f, 0.2f, 0.9f,	   EaseOutCubic());
}

void IGame::screenshotDraw()
{
	gl::pushMatrices();
	gl::translate(animX1, 0.0f);
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
	if (screenshot)
	{
		gl::draw(screenshot);
	}
	gl::color(Color::white());
	gl::popMatrices();
}