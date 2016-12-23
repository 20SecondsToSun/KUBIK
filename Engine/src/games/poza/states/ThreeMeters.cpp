#include "ThreeMeters.h"
#include "games/poza/Poza.h"


using namespace std;
using namespace ci;
using namespace ci::signals;
using namespace kubik::games::poza;
using namespace kubik;
using namespace kubik::config;

ThreeMeters::ThreeMeters(PozaSettingsRef settings)
	:animTime(0.8f),
	alphaAnim(1.0f),
	DISTANCE_OK(3)
{
	titlePositionY = 237.0f;
	voidBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(1080, 1920, Vec2f(0.0f, 80.0f)));
	voidBtn->setAlpha(1.0f);
	reset(settings);
}

ThreeMeters::~ThreeMeters()
{
	logger().log("~~~ Poza.SubLocation ThreeMeters.Destruct  ~~~");
}

void ThreeMeters::reset(ISettingsRef set)
{
	logger().log("~~~ Poza.SubLocation ThreeMeters.Reset ~~~");

	IGameLocation::reset(set);
	settings = set;

	thrimeters = settings->getTexture("3meters");
	arrow = settings->getTexture("arrow");

	titleTexPos = Vec2f(0.5f * (app::getWindowWidth() - thrimeters.getWidth()), titlePositionY	   - thrimeters.getHeight() * 0.5f);
	arrowTexPos = Vec2f(0.5f * (app::getWindowWidth() - arrow.getWidth()), 590);// (app::getWindowHeight() - arrow.getHeight()) * 0.5f);

	DISTANCE_OK = static_pointer_cast<PozaSettings>(settings)->DISTANCE_OK;
}

void ThreeMeters::start()
{
	logger().log("~~~ Poza.SubLocation ThreeMeters.Start ~~~");

	voidBtn->connectEventHandler(&ThreeMeters::hideAnimation, this);
	delaycall(bind(&ThreeMeters::initAnimationcomplete, this), 0.4f);

	if (kinect().deviceExist())
	{
		kinect().getDevice()->connectBodyEventHandler([&](const Kinect2::BodyFrame frame)
		{
			mBodyFrame = frame;
		});

		kinect().getDevice()->connectBodyIndexEventHandler([&](const Kinect2::BodyIndexFrame frame)
		{
			mChannelBodyIndex = frame.getChannel();
		});

		kinect().getDevice()->connectDepthEventHandler([&](const Kinect2::DepthFrame frame)
		{
			mChannelDepth = frame.getChannel();
		});
	}
}

void ThreeMeters::initAnimationcomplete()
{
	callback(COMPLETE_ANIM);
}

void ThreeMeters::stop()
{
	logger().log("~~~ Poza.SubLocation ThreeMeters.Stop ~~~");

	stopAllTweens();
	voidBtn->disconnectEventHandler();

	kinect().getDevice()->disconnectBodyEventHandler();
	kinect().getDevice()->disconnectBodyIndexEventHandler();
	kinect().getDevice()->disconnectDepthEventHandler();
}

void ThreeMeters::hideAnimation(EventGUIRef& event)
{
	callback(BEGIN_ANIM);
	app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&ThreeMeters::hideAnimationComplete, this));
}

void ThreeMeters::hideAnimationComplete()
{
	alphaAnim = 1.0f;
	IGameLocation::hideAnimationComplete();
}

void ThreeMeters::update()
{
	if (mChannelDepth)
	{
		kinect().calcCenterBody(mBodyFrame, mChannelDepth.getSize());
		bodies = kinect().getFilterBodies();
		if (bodies.size())
		{
			centerBody = kinect().getCenterBody();

			if (centerBody.distanceZ > DISTANCE_OK)
			{
				callback(BEGIN_ANIM);
				app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&ThreeMeters::hideAnimationComplete, this));
			}
		}
	}
}

void ThreeMeters::draw()
{
	fillBg();
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(thrimeters, Vec2i(titleTexPos));
	gl::draw(arrow, Vec2i(arrowTexPos));

	if (mChannelDepth)
	{
		if (bodies.size())
		{			
			drawPersonPoiint(centerBody, Color(1, 0, 0));			
		}

		for (size_t i = 0; i < bodies.size(); i++)
		{
			if (!bodies[i].isCentered)
			{	
				drawPersonPoiint(bodies[i], Color(1, 1, 1));
			}			
		}
	}
}

void ThreeMeters::drawPersonPoiint(KinectAdapter::BodyFilter& body, const ci::Color& color)
{	
	gl::color(color);
	gl::pushMatrices();

	gl::translate(body.getFitPosition(mChannelDepth.getSize(), Poza::kinectShiftY));
	gl::drawSolidCircle(Vec2f::zero(), 100, 20);
	gl::color(Color(0, 1, 0));

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << body.distanceZ;
	std::string distanceZ = ss.str();

	Utils::textFieldDraw(distanceZ, &getFont("IntroLight", 120), Vec2f::zero(), Color(0, 1, 0));
	gl::popMatrices();
	gl::color(Color(1, 1, 1));
}

void ThreeMeters::stopAllTweens()
{
	alphaAnim.stop();
	IGameLocation::stopAllTweens();
}