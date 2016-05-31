#include "HandsUp.h"
#include "Poza.h"

using namespace std;
using namespace ci;
using namespace ci::signals;
using namespace kubik::games::poza;
using namespace kubik;
using namespace kubik::config;

HandsUp::HandsUp(PozaSettingsRef settings, shared_ptr<HumanModel> humanModel)
	:animTime(0.8f),
	alphaAnim(1.0f),
	humanModel(humanModel)
{
	titlePositionY = 237.0f;
	voidBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(1080, 1920, Vec2f(0.0f, 80.0f)));
	voidBtn->setAlpha(1.0f);
	reset(settings);
}

HandsUp::~HandsUp()
{
	logger().log("~~~ Poza.SubLocation HandsUp.Destruct ~~~");
}

void HandsUp::reset(ISettingsRef set)
{
	logger().log("~~~ Poza.SubLocation HandsUp.Reset ~~~");

	IGameLocation::reset(set);
	settings = set;

	titlte = settings->getTexture("handsup");
	man = settings->getTexture("man");

	titleTexPos = Vec2f(0.5f * (app::getWindowWidth() - titlte.getWidth()), titlePositionY - titlte.getHeight() * 0.5f);
	manTexPos   = Vec2f(0.5f * (app::getWindowWidth() - man.getWidth()), 464.0f);
}

void HandsUp::start()
{
	logger().log("~~~ Poza.SubLocation HandsUp.Start ~~~");

	voidBtn->connectEventHandler(&HandsUp::hideAnimation, this);
	delaycall(bind(&HandsUp::initAnimationcomplete, this), 0.4f);

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

	detected = false;
}

void HandsUp::initAnimationcomplete()
{
	callback(COMPLETE_ANIM);
}

void HandsUp::stop()
{
	logger().log("~~~ Poza.SubLocation HandsUp.Stop ~~~");

	stopAllTweens();
	disconnectAllListeners();	
}

void HandsUp::handsUpDetectionHandler()
{
	callback(BEGIN_ANIM);
	app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&HandsUp::hideAnimationComplete, this));
}

void HandsUp::hideAnimation(EventGUIRef& event)
{
	callback(BEGIN_ANIM);
	app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&HandsUp::hideAnimationComplete, this));
}

void HandsUp::hideAnimationComplete()
{
	alphaAnim = 1.0f;
	IGameLocation::hideAnimationComplete();
}

void HandsUp::update()
{
	if (mChannelDepth && !detected)
	{
		kinect().calcCenterBody(mBodyFrame, mChannelDepth.getSize());
		auto bodies = kinect().getFilterBodies();

		if (bodies.size())
		{
			auto centerBody = kinect().getCenterBody();
			auto head = centerBody.body.getJointMap().at(JointType::JointType_Head);
			auto lHand = centerBody.body.getJointMap().at(JointType::JointType_HandRight);
			auto rHand = centerBody.body.getJointMap().at(JointType::JointType_HandLeft);

			auto posHead = Vec2f(kinect().getDevice()->mapCameraToDepth(head.getPosition()));
			auto posLeftHand = Vec2f(kinect().getDevice()->mapCameraToDepth(lHand.getPosition()));
			auto posRightHand = Vec2f(kinect().getDevice()->mapCameraToDepth(rHand.getPosition()));

			if ((head.getTrackingState()  != TrackingState::TrackingState_NotTracked) &&
				(lHand.getTrackingState() != TrackingState::TrackingState_NotTracked) &&
				(rHand.getTrackingState() != TrackingState::TrackingState_NotTracked))
			{
				if (posHead.y > posLeftHand.y && posHead.y > posRightHand.y)
				{					
					auto leftFoot = centerBody.body.getJointMap().at(JointType::JointType_AnkleLeft);
					auto rightFoot = centerBody.body.getJointMap().at(JointType::JointType_AnkleRight);

					auto posLeftFoot  = Vec2f(kinect().getDevice()->mapCameraToDepth(leftFoot.getPosition()));
					auto posRightFoot = Vec2f(kinect().getDevice()->mapCameraToDepth(rightFoot.getPosition()));

					if ((leftFoot.getTrackingState() != TrackingState::TrackingState_NotTracked) &&
						(rightFoot.getTrackingState() != TrackingState::TrackingState_NotTracked))
						{
							detected = true;
							auto spine = centerBody.body.getJointMap().at(JointType::JointType_SpineBase);
						
							humanModel->height   = abs(posLeftFoot.y - posHead.y);
							humanModel->distance = spine.getPosition().z;
							humanModel->calculateHeightScale();
							handsUpDetectionHandler();
						}
				}
			}
		}
	}
}

void HandsUp::draw()
{
	fillBg();

	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));
	gl::draw(titlte, Vec2i(titleTexPos));
	gl::draw(man, Vec2i(manTexPos));
}

void HandsUp::stopAllTweens()
{
	alphaAnim.stop();
	IGameLocation::stopAllTweens();
}

void HandsUp::disconnectAllListeners()
{
	voidBtn->disconnectEventHandler();
	kinect().getDevice()->disconnectBodyEventHandler();
	kinect().getDevice()->disconnectBodyIndexEventHandler();
	kinect().getDevice()->disconnectDepthEventHandler();
}