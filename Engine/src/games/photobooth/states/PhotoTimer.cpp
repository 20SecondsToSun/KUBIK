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
	settings =  sett;	

	timerFont = settings->getFont("introThin120");
	MAX_SEC = settings->getBeReadySeconds();

	auto title1   = settings->getTextItem(PhtTextID::TIMER_TEXT1);
	auto title2   = settings->getTextItem(PhtTextID::TIMER_TEXT2);

	tex1 = textTools().getTextField(title1);
	tex2 = textTools().getTextField(title2);

	title1Pos = Vec2f(0.5f * (getWindowWidth() - tex1.getWidth()), 340.0f);
	title2Pos = Vec2f(0.5f * (getWindowWidth() - tex2.getWidth()), 468.0f);

	timerTex1 = settings->getTexture("timer1");
	timerTex2 = settings->getTexture("timer2");

	timerTexPos = Vec2f(0.5 * (getWindowWidth() - timerTex1.getWidth()), 706);//806
}

void PhotoTimer::start()
{
	seconds = MAX_SEC;
	cdTimer.start();
	changeAngle = 0;
	rotor = - (startAngle - endAngle) / MAX_SEC;
	console()<<"start PhotoTimer"<<endl;	
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

	//cameraCanon().update();
}	

void PhotoTimer::draw()
{
	fillBg();
	gl::draw(tex1, title1Pos);
	gl::draw(tex2, title2Pos);	

	Texture tex = textTools().getTextField(to_string(seconds), &timerFont, ci::Color::white());
	
	float x  = 0.5 * (getWindowWidth() - tex.getWidth());
	float y  = timerTexPos.y + 0.5 * (timerTex1.getHeight() - tex.getHeight());
	float y1 = timerTexPos.y + 0.5 * timerTex1.getHeight();

	gl::draw(tex, Vec2f(x, y));
	gl::draw(timerTex1, timerTexPos);		
	gl::Texture texMask = drawtool().circleSliceTexture(getWindowCenter().x, y1, RADIUS, startAngle, endAngle + changeAngle, true);	
	maskShader->render(timerTex2, texMask, timerTexPos);

	Sprite::draw();
}