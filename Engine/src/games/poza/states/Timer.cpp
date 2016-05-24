#include "Timer.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace kubik::games::poza;
using namespace kubik;
using namespace kubik::config;

PozaTimer1::PozaTimer1(PozaSettingsRef settings)
	:animTime(0.8f),
	alphaAnim(1.0f)
{
	titlePositionY = 425.f;
	reset(settings);
}

PozaTimer1::~PozaTimer1()
{
	logger().log("~~~ Poza.SubLocation Timer.Destruct ~~~");
}

void PozaTimer1::reset(ISettingsRef set)
{
	logger().log("~~~ Poza.SubLocation Timer.Reset ~~~");
	IGameLocation::reset(set);
	settings = set;

	MAX_SEC = 5;// settings->PhotoSeconds;

	title2 = settings->getTexture("title2");
	circles = settings->getTexture("circles");

	titleTexPos   = Vec2f(0.5f * (app::getWindowWidth() - title2.getWidth()), titlePositionY - title2.getHeight() * 0.5f);
	circlesTexPos = Vec2f(0.5f * (app::getWindowWidth() - circles.getWidth()), (app::getWindowHeight() - circles.getHeight()) * 0.5f);

	digits.clear();
	for (size_t i = DIGIT_COUNT; i > 0; i--)
	{
		digits.push_back(settings->getTexture("digit" + to_string(i)));
	}
}

void PozaTimer1::start()
{
	logger().log("~~~ Poza.SubLocation Timer.Start ~~~");
	alphaAnim = 1.0f;
	delaycall(bind(&PozaTimer1::initAnimationcomplete, this), 0.4f);

	seconds = MAX_SEC;
	digitScale = 0.0f;
	timeline().apply(&digitScale, 0.2f, 1.0f, 0.8f, EaseOutBack(2.40158f)).delay(0.25f);

	state = SHOW_ANIM;
}

void PozaTimer1::initAnimationcomplete()
{
	logger().log("initAnimationcomplete");
	cdTimer.start();
	state = COUNTING;
	callback(COMPLETE_ANIM);
}

void PozaTimer1::stop()
{
	logger().log("~~~ Poza.SubLocation Timer.Stop ~~~");
	cdTimer.stop();
	digitScale.stop();
	alphaAnim.stop();
	clearDelaycall();
	stopAllTweens();
}

void PozaTimer1::hideAnimation(EventGUIRef& event)
{
	callback(BEGIN_ANIM);
	app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&PozaTimer1::hideAnimationComplete, this));
}

void PozaTimer1::hideAnimationComplete()
{	
	IGameLocation::hideAnimationComplete();
}

void PozaTimer1::update()
{
	switch (state)
	{
	case PozaTimer1::SHOW_ANIM:
		currentDigit = digits[0];
		break;

	case PozaTimer1::COUNTING:
		calculateDigit();
		break;

	case PozaTimer1::HIDE_ANIM:
		currentDigit = digits[DIGIT_COUNT - 1];
		break;
	}
}

void PozaTimer1::calculateDigit()
{
	float timersec = cdTimer.getSeconds();
	seconds = (MAX_SEC - (int)timersec);	

	int index = MAX_SEC - seconds;

	if (index > DIGIT_COUNT - 1)
	{
		index = DIGIT_COUNT - 1;
	}

	seconds = (MAX_SEC - (int)cdTimer.getSeconds());

	if (seconds <= 1.4)
	{
		cdTimer.stop();
		state = PozaTimer1::HIDE_ANIM;
		initHideAnimationParams();
	}

	currentDigit = digits[index];
}

void PozaTimer1::initHideAnimationParams()
{
	callback(BEGIN_ANIM);
	timeline().apply(&alphaAnim, 1.0f, 0.0f, 0.5f, EaseInBack(2.70158f))
		.finishFn(bind(&PozaTimer1::hideAnimationComplete, this));
}

void PozaTimer1::draw()
{
	fillBg();
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(title2,  Vec2i(titleTexPos));
	gl::draw(circles, Vec2i(circlesTexPos));
	drawDigit();
}

void PozaTimer1::drawDigit()
{
	if (currentDigit)
	{
		auto centerDigit = currentDigit.getSize() * 0.5f;
		gl::pushMatrices();
		gl::translate(Vec2f(0.5f * (getWindowWidth() - currentDigit.getWidth()), centerY - 0.5f * currentDigit.getHeight()));
		gl::translate(centerDigit);
		gl::scale(digitScale, digitScale);
		gl::translate(-centerDigit);
		gl::draw(currentDigit);
		gl::popMatrices();
	}
}

void PozaTimer1::stopAllTweens()
{
	alphaAnim.stop();
	IGameLocation::stopAllTweens();
}