#include "PhotoTimer.h"

using namespace std;
using namespace ci;
using namespace kubik;
using namespace kubik::games::photobooth;
using namespace shaders::imagefilters;

void PhotoTimer::clear()
{
	cdTimer.stop();
}

PhotoTimer::~PhotoTimer()
{
	clear();
};

PhotoTimer::PhotoTimer(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage)
	:photoStorage(photoStorage),
	startAngle(90),
	endAngle(-270),
	changeAngle(0),
	RADIUS(500)
{
	reset(settings);
	maskShader = shadertool().getMaskShader();
}

void PhotoTimer::reset(PhotoboothSettingsRef sett)
{
	IPhotoboothLocation::reset(settings);

	MAX_SEC = settings->getBeReadySeconds();

	title = settings->getTexture("timertitle");
	titlePos = Vec2f(0.5f * (getWindowWidth() - title.getWidth()), 432.0f - title.getHeight() * 0.5f);

	timerTex1 = settings->getTexture("timer1");
	timerTex2 = settings->getTexture("timer2");

	digits.clear();
	digits.push_back(settings->getTexture("digit4"));
	digits.push_back(settings->getTexture("digit3"));
	digits.push_back(settings->getTexture("digit2"));
	digits.push_back(settings->getTexture("digit1"));

	timerTexPos = Vec2f(0.5f * (getWindowWidth() - timerTex1.getWidth()), centerY - timerTex1.getHeight() * 0.5f);
}

void PhotoTimer::start()
{
	console() << "start PhotoTimer" << endl;

	seconds = MAX_SEC;
	changeAngle = 0;
	rotor = -(startAngle - endAngle) / MAX_SEC;
	initShowAnimationParams();
	state = SHOW_ANIM;
}

void PhotoTimer::initShowAnimationParams()
{
	titleAlpha = 0.1f;
	titleAnimPosition = Vec2f(titlePos.x, titlePositionY - 170.0f);
	timeline().apply(&titleAlpha, 1.0f, animShowTitleTime + 0.2f);// , EaseOutQuart());
	timeline().apply(&titleAnimPosition, Vec2f(titlePos.x, titlePositionY), animShowTitleTime, EaseOutExpo())
		.finishFn(bind(&PhotoTimer::showAnimationComplete, this));

	timeline().apply(&circleScale, 0.3f, 1.0f, 0.8f, EaseOutBack(2.40158f));
	digitScale = 0.0f;
	timeline().apply(&digitScale, 0.2f, 1.0f, 0.8f, EaseOutBack(2.40158f)).delay(0.25f);
}

void PhotoTimer::showAnimationComplete()
{
	cdTimer.start();
	state = COUNTING;
}

void PhotoTimer::stop()
{
	cdTimer.stop();
	stopAllTweens();
}

void PhotoTimer::update()
{
	switch (state)
	{
	case PhotoTimer::SHOW_ANIM:
		currentDigit = digits[0];
		break;

	case PhotoTimer::COUNTING:
		calculateDigit();
		break;

	case PhotoTimer::HIDE_ANIM:
		currentDigit = digits[MAX_SEC - 1];
		break;
	default:
		break;
	}
}

void PhotoTimer::calculateDigit()
{
	float timersec = cdTimer.getSeconds();
	seconds = (MAX_SEC - (int)timersec);
	changeAngle = rotor * timersec;

	int index = MAX_SEC - seconds;
	if (index >= MAX_SEC)
		index = MAX_SEC - 1;

	seconds = (MAX_SEC - (int)cdTimer.getSeconds());

	if (seconds < 0)
	{
		cdTimer.stop();
		state = PhotoTimer::HIDE_ANIM;
		initHideAnimationParams();
	}

	currentDigit = digits[index];
}

void PhotoTimer::initHideAnimationParams()
{
	timeline().apply(&circleScale, 1.0f, 0.5f, 0.5f, EaseInBack(2.70158f)).delay(0.15f)
		.finishFn(bind(&PhotoTimer::hideAnimationComplete, this));
	timeline().apply(&digitScale, 0.0f, 0.5f, EaseInBack(2.70158f));
}

void PhotoTimer::hideAnimationComplete()
{
	auto photo = Utils::drawGraphicsToFBO(1080, 1920, [&](){ draw(); });
	photoStorage->setLastScreenShot(photo);
	nextLocationSignal();
}

void PhotoTimer::draw()
{
	fillBg();
	drawTitle();
	drawAnimationCircle();
	drawDigit();
	Sprite::draw();
}

void PhotoTimer::drawAnimationCircle()
{
	float y = timerTexPos.y + 0.5f * timerTex1.getHeight();
	gl::Texture texMask = drawtool().circleSliceTexture(getWindowCenter().x, y, RADIUS, startAngle, endAngle + changeAngle, true);

	auto photo = Utils::drawGraphicsToFBO(timerTex2.getSize(), [&]()
	{
		maskShader->render(timerTex2, texMask, Vec2f::zero(), 1);
	});

	auto centerTex = timerTex1.getSize()*0.5f;

	gl::color(ColorA(1.0f, 1.0f, 1.0f, titleAlpha));
	gl::pushMatrices();
	gl::translate(timerTexPos);
	gl::translate(centerTex);
	gl::scale(circleScale, circleScale);
	gl::translate(-centerTex);
	gl::draw(timerTex1);
	gl::draw(photo);
	gl::popMatrices();
	gl::color(Color::white());
}

void PhotoTimer::drawDigit()
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

void PhotoTimer::stopAllTweens()
{
	circleScale.stop();
	digitScale.stop();
	IPhotoboothLocation::stopAllTweens();
}