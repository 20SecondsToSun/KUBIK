#include "PhotoTimer.h"

using namespace std;
using namespace ci;
using namespace kubik;
using namespace kubik::games;
using namespace shaders::imagefilters;

void PhotoTimer::clear()
{
	cdTimer.stop();
}

PhotoTimer::~PhotoTimer()
{
	clear();
};

PhotoTimer::PhotoTimer(PhotoboothSettingsRef settings):
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

	centerY = 1123.0f;
	timerTexPos = Vec2f(0.5f * (getWindowWidth() - timerTex1.getWidth()), centerY - timerTex1.getHeight() * 0.5f);//806
}

void PhotoTimer::start()
{
	seconds = MAX_SEC;
	cdTimer.start();
	changeAngle = 0;
	rotor = - (startAngle - endAngle) / MAX_SEC;
	console()<<"start PhotoTimer"<< MAX_SEC<<endl;	
}	

void PhotoTimer::stop()
{
	cdTimer.stop();
}

void PhotoTimer::update()
{
	seconds = (MAX_SEC - (int)cdTimer.getSeconds());
	changeAngle = rotor * cdTimer.getSeconds() ;
	if(seconds < 0)
	{
		cdTimer.stop();
		nextLocationSignal();
	}	
}	

void PhotoTimer::draw()
{
	fillBg();

	gl::draw(title, titlePos);

	drawAnimationCircle();	

	int index = MAX_SEC - seconds;
	if(index >= MAX_SEC) index = MAX_SEC - 1;
	auto digit = digits[index];
	gl::draw(digit, Vec2f(0.5f*(getWindowWidth() - digit.getWidth()), centerY - 0.5f * digit.getHeight()));

	Sprite::draw();
}

void PhotoTimer::drawAnimationCircle()
{
	float y = timerTexPos.y + 0.5 * timerTex1.getHeight();
	gl::draw(timerTex1, timerTexPos);		
	gl::Texture texMask = drawtool().circleSliceTexture(getWindowCenter().x, y, RADIUS, startAngle, endAngle + changeAngle, true);	
	maskShader->render(timerTex2, texMask, timerTexPos);	
}