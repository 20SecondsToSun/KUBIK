#include "PozaGame.h"
#include "TextTools.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace kubik::games::poza;
using namespace kubik;
using namespace kubik::config;

PozaGame::PozaGame(PozaSettingsRef settings, kubik::games::photobooth::PhotoStorageRef  photoStorage, std::vector<int>& gameScore, PozaBase& pozaData, shared_ptr<HumanModel> humanModel)
	:photoStorage(photoStorage),
	animTime(0.8f),
	alphaAnim(1.0f),
	numsFont(getFont("IntroLight", 120)),
	gameScore(gameScore),
	pozaData(pozaData),
	humanModel(humanModel)
{
	titlePositionY  = 425.f;
	cameraScale		= 1920.0f / 1056.0f;
	cameraWidth		= 1056 * cameraScale;
	cameraHeight	= 704 * cameraScale;
	cameraPosition	= Vec2f((1080.0f - cameraHeight) * 0.5f, 0.0f);

	if (!photoTakenCon.connected())
	{
		photoTakenCon = cameraCanon().photoTakenEvent.connect(bind(&PozaGame::photoTakenHandler, this));
	}

	if (!photoDownloadedCon.connected())
	{
		photoDownloadedCon = cameraCanon().photoDownloadedEvent.connect(bind(&PozaGame::photoDownloadHandler, this, std::placeholders::_1));
	}

	if (!photoErrorCon.connected())
	{
		photoErrorCon = cameraCanon().photoErrorEvent.connect(bind(&PozaGame::photoErrorHandler, this));
	}

	reset(settings);	
	voidBtn = SimpleSpriteButtonRef(new SimpleSpriteButton(1080, 1920, Vec2f(0.0f, 80.0f)));
}

PozaGame::~PozaGame()
{
	logger().log("~~~ Poza.SubLocation PozaGame.Destruct  ~~~");
	photoTakenCon.disconnect();
	photoDownloadedCon.disconnect();
	photoErrorCon.disconnect();
}

void PozaGame::reset(ISettingsRef set)
{
	logger().log("~~~ Poza.SubLocation ThreeMeters.Reset ~~~");
	 
	IGameLocation::reset(set);
	settings = set;

	fail		= settings->getTexture("fail");
	controls	= settings->getTexture("controls");
	good		= settings->getTexture("good");
	bad			= settings->getTexture("bad");
	returnTitle = settings->getTexture("return");
	circles		= settings->getTexture("circles");

	titleTexPos   = Vec2f(0.5f * (app::getWindowWidth() - returnTitle.getWidth()), titlePositionY - returnTitle.getHeight()   * 0.5f);
	circlesTexPos = Vec2f(0.5f * (app::getWindowWidth() - circles.getWidth()), (app::getWindowHeight() - circles.getHeight()) * 0.5f);
	
	DIGIT_COUNT = 3;
	MAX_SEC = 3;
	onePozaTime = 100;
	controlsPos = Vec2f(0.5f * (app::getWindowWidth() - controls.getWidth()), app::getWindowHeight() - controls.getHeight());

	gameScore.clear();
	gameScore.push_back(0);
	gameScore.push_back(0);
	gameScore.push_back(0);

	digits.clear();
	for (size_t i = 3; i > 0; i--)
	{
		digits.push_back(settings->getTexture("digit" + to_string(i)));
	}

	auto pozasettings = static_pointer_cast<PozaSettings>(set);
	currentCardTemplate = pozasettings->getCurrentTemplate();
}

void PozaGame::start()
{
	logger().log("~~~ Poza.SubLocation PozaGame.Start ~~~");

	//humanModel->scaleAccordingUserHeight;
	auto scaleFactor = 1;
	poza = pozaData.getScaledPose(scaleFactor);

	levelCompletion = 0;
	pozaNum = 1;	
	photoStorage->clear();	
	
	delaycall(bind(&PozaGame::initAnimationcomplete, this), 0.4f);
	state = STARTING;
}

void PozaGame::initAnimationcomplete()
{
	callback(COMPLETE_ANIM);
	cdTimer.start();
	
	cameraCanon().startLiveView();
	voidBtn->connectEventHandler(&PozaGame::pozaSuccessHandler, this);

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

		kinect().getDevice()->connectColorEventHandler([&](const Kinect2::ColorFrame& frame)
		{
			mSurfaceColor = frame.getSurface();
		});
	}

	kinect().enableHandTracking();
	state = GAME;
}

void PozaGame::pozaSuccessHandler(EventGUIRef& event)
{
	//state = SHOOTING;
	//cdTimer.stop();
	//cameraCanon().takePicture();	
	colorPoint++;
}

void PozaGame::photoTakenHandler()
{
	logger().log("~~~ Poza.SubLocation Game.PhotoTaken!!! ~~~");
	dbRecord->CameraShotsNum++;
}

void PozaGame::photoDownloadHandler(const string& path)
{
	logger().log("~~~ Poza.SubLocation Game.Downloaded ~~~");
	dbRecord->CameraGoodShotsNum++;
	photoStorage->loadDownloadedPhoto(path);
	photo = photoStorage->mergeLastPhotoWith(poza.comicsTex);

	state = PREVIEW;
	previewAnimateX = -1800;
	timeline().apply(&previewAnimateX, 0.0f, 1.7f, EaseInExpo()).delay(1.4f);
	roundFinished(true);
}

void PozaGame::photoErrorHandler()
{
	logger().log("~~~ Poza.SubLocation Error ~~~");

	photoStorage->createFormat2Template(cameraTexture);
	photoStorage->setNextPhoto(cameraTexture);
	photo = photoStorage->mergeLastPhotoWith(poza.comicsTex);

	state = PREVIEW;
	previewAnimateX = -1800;
	timeline().apply(&previewAnimateX, .0f, 1.7f, EaseInExpo()).delay(1.4f);
	roundFinished(true);
}

void PozaGame::previewDelayHandler()
{
	cameraCanon().startLiveView();
	state = GAME;
}

void PozaGame::stop()
{
	logger().log("~~~ Poza.SubLocation PozaGame.Stop ~~~");
	stopAllTweens();

	voidBtn->disconnectEventHandler();
	if (kinect().deviceExist())
	{
		kinect().getDevice()->disconnectBodyEventHandler();
		kinect().getDevice()->disconnectBodyIndexEventHandler();
		kinect().getDevice()->disconnectDepthEventHandler();
		kinect().getDevice()->disconnectColorEventHandler();
	}
}

void PozaGame::hideAnimation(EventGUIRef& event)
{
	callback(BEGIN_ANIM);
	app::timeline().apply(&alphaAnim, 0.0f, animTime, EaseOutCubic()).finishFn(bind(&PozaGame::hideAnimationComplete, this));
}

void PozaGame::hideAnimationComplete()
{
	alphaAnim = 1.0f;
	IGameLocation::hideAnimationComplete();
}

void PozaGame::update()
{
	switch (state)
	{
		case PozaGame::STARTING:
		{
			break;
		}		

		case PozaGame::GAME:
		{
			cameraCanon().update();
			updateJointsPosition();
			matchPozaTemplate();
			float timersec = cdTimer.getSeconds();
			seconds = (onePozaTime - (int)timersec);

			if (seconds <= 0)
			{
				state = FAIL;
				roundFinished(false);
			}
			checkForPoseGuess();			
			break;
		}

		case PozaGame::SHOOTING:
		{
			break;
		}	

		case PozaGame::FAIL:
		{
			break;
		}

		case PozaGame::PREGAME:
		{
			calculateDigit();
			break;
		}

		case PozaGame::HIDE_ANIM:
		{
			break;
		}
	}
}

void PozaGame::updateJointsPosition()
{
	humanPoints.clear();

	if (mChannelBodyIndex)
	{
		gl::pushMatrices();
		auto scale = getWindowSize() / mChannelBodyIndex.getSize();
		gl::disable(GL_TEXTURE_2D);
		for (const Kinect2::Body& body : mBodyFrame.getBodies())
		{
			if (body.isTracked())
			{
				gl::color(Color(1, 0, 0));
				for (const auto& joint : body.getJointMap())
				{
					if (joint.first == JointType::JointType_ThumbLeft    ||
						joint.first == JointType::JointType_ThumbRight   ||
						joint.first == JointType::JointType_HandTipLeft  ||
						joint.first == JointType::JointType_HandTipRight ||
						joint.first == JointType::JointType_HandLeft     ||
						joint.first == JointType::JointType_HandRight)
					{
						continue;
					}

					if (joint.second .getTrackingState() == TrackingState::TrackingState_Tracked)
					{
						auto scale = 1920.0f / mChannelDepth.getHeight();
						auto shift = Vec2f(0.5f * (1080.0f - scale * mChannelDepth.getWidth()), kinectShiftY);
						auto pos = Vec2f(kinect().getDevice()->mapCameraToDepth(joint.second.getPosition()));
						humanPoints.push_back(pos * scale + shift);
					}
				}
				gl::color(Color(1, 1, 1));
			}
		}
		gl::popMatrices();
	}
}

void PozaGame::matchPozaTemplate()
{
	mathPercent = 0.0f;
	auto pozaTemplatePoints = poza.points;
	auto maxErrorBetweenJoints = 40.0f;
	auto minErrorBetweenJoints = 10.0f;

	auto calculateDistanceBetweenPoints = [&](const Vec2f& vec1, const Vec2f& vec2)
	{
		return abs((vec1- vec2).length());
	};

	if (pozaTemplatePoints.size() != humanPoints.size())
	{
		return;
	}

	for (size_t j = 0, len = pozaTemplatePoints.size(); j < len; ++j)
	{
		double mistake = calculateDistanceBetweenPoints(pozaTemplatePoints[j], humanPoints[j]);
		double onePartPercent = 0;
		double onePart = 1 / len;// Params::weightJoints[j];

		if (mistake >= maxErrorBetweenJoints)
		{
			onePartPercent = 0;
		}
		else if (mistake < minErrorBetweenJoints)
		{
			//currentPose.setPointColor(j, ColorA(1.0f, 1.0f, 1.0f, 0.0f));
			onePartPercent = onePart;
		}
		else
		{
			float norma = (maxErrorBetweenJoints - mistake) / (maxErrorBetweenJoints - minErrorBetweenJoints);
			onePartPercent = onePart * (1 - norma);
			//currentPose.setPointColor(j, ColorA(0.0f, 0.0f, 1.0f, abs(onePartPercent * 10)));
		}

		mathPercent += onePartPercent;
	}
}

void PozaGame::draw()
{
	fillBg();
	gl::color(ColorA(1.0f, 1.0f, 1.0f, alphaAnim));

	switch (state)
	{
		case PozaGame::STARTING:
		{
			gl::draw(controls, Vec2i(controlsPos));
			break;
		}

		case PozaGame::GAME:
		{
			drawCameraLayer();
			drawKinectStream();
			drawCounturLayer();			
			drawCurrentPosePoints();			

			gl::draw(controls, Vec2i(controlsPos));
			drawCircles();
			drawTimer();	
			drawProgressBar();	
			break;
		}

		case PozaGame::SHOOTING:
		{
			break;
		}

		case PozaGame::PREVIEW:
		{			
			drawPhotoframe();
			break;
		}

		case PozaGame::FAIL:
		{
			drawFailImage();
			break;
		}

		case PozaGame::PREGAME:
		{
			gl::draw(returnTitle, Vec2i(titleTexPos));
			gl::draw(circles, Vec2i(circlesTexPos));
			gl::pushMatrices();
			gl::translate(Vec2f(0.5f * (getWindowWidth() - currentDigit.getWidth()), 0.5f * (getWindowHeight() - currentDigit.getHeight())));
			gl::draw(currentDigit);
			gl::popMatrices();
			break;
		}

		case PozaGame::HIDE_ANIM:
		{
			break;
		}
	}			
}

void PozaGame::drawCameraLayer()
{
	cameraTexture = cameraCanon().getTexture(cameraHeight, cameraWidth, 0, 0, cameraScale);
	gl::draw(cameraTexture, cameraPosition);
}

void PozaGame::drawKinectStream()
{
	auto drawColorStream = [&]() -> void
	{
		if (mSurfaceColor)
		{
			gl::TextureRef tex = gl::Texture::create(mSurfaceColor);
			gl::color(ColorAf(Colorf::white(), 1.f));
			gl::draw(mSurfaceColor);
		}
	};

	auto drawDepthStream = [&]() -> void
	{
		if (mChannelDepth)
		{
			gl::enable(GL_TEXTURE_2D);
			gl::TextureRef tex = gl::Texture::create(Kinect2::channel16To8(mChannelDepth));
			gl::draw(tex);
			gl::disable(GL_TEXTURE_2D);
		}
	};

	auto drawBodyIndexStream = [&]() -> void
	{
		if (mChannelBodyIndex)
		{
			gl::enable(GL_TEXTURE_2D);
			gl::color(ColorAf(Colorf::white(), 0.35f));
			gl::TextureRef bodyColorSurf = gl::Texture::create(Kinect2::colorizeBodyIndex(mChannelBodyIndex));
			gl::draw(bodyColorSurf);
			gl::disable(GL_TEXTURE_2D);
			gl::color(ColorAf(Colorf::white(), 1.f));
		}
	};
	
	if (mChannelDepth)
	{
		gl::pushMatrices();
		auto scale = 1920.0f / mChannelDepth.getHeight();		
		auto shift = Vec2f(0.5 * (1080 - scale * mChannelDepth.getWidth()), 0);

		gl::translate(shift);
		gl::scale(scale, scale);
		//drawDepthStream();
		drawBodyIndexStream();
		gl::popMatrices();
	}
}

void PozaGame::drawPhotoframe()
{
	auto startY = 294.0f;
	auto _scale1 = 748.0f / currentCardTemplate.getWidth();
	gl::pushMatrices();

	gl::translate(previewAnimateX, 0.0f);

	gl::pushMatrices();
	gl::scale(_scale1, _scale1);	
	gl::translate(0.5f * (getWindowWidth() * (1.0f / _scale1) - currentCardTemplate.getWidth()), startY + 235);
	gl::draw(currentCardTemplate);
	gl::popMatrices();

	_scale1 = 748.0f / photo.getWidth();
	gl::pushMatrices();
	gl::scale(_scale1, _scale1);
	gl::translate(0.5f * (getWindowWidth() * (1.0f / _scale1) - photo.getWidth()), startY + 235);
	gl::draw(photo);
	gl::popMatrices();
	gl::popMatrices();
}

void PozaGame::roundFinished(bool isGood)
{
	gameScore[pozaNum - 1] = isGood ? 1 : 0;
	cdTimer.stop();
	delaycall(bind(&PozaGame::newRoundInit, this), 5.0f);
}

void PozaGame::newRoundInit()
{
	timeline().apply(&previewAnimateX, 2000.0f, 1.0f, EaseInExpo()).finishFn(bind(&PozaGame::cardAnimationOutHandler, this));	
}

void PozaGame::cardAnimationOutHandler()
{
	if (pozaNum < 3)
	{
		poza = pozaData.getPose();
		pozaNum++;
		levelCompletion = 0;
		cdTimer.stop();
		pregameTimer.start();
		state = PREGAME;
		cameraCanon().startLiveView();
	}
	else
	{
		photoStorage->setAllSelected();
		photoStorage->createChoosingPreview();
		photoStorage->createPhotoTemplates();
		nextLocationSignal();
	}
}

void PozaGame::calculateDigit()
{
	float timersec = pregameTimer.getSeconds();
	seconds = (MAX_SEC - (int)timersec);

	//changeAngle = Utils::clamp(rotor * timersec, 0, -360);

	int index = MAX_SEC - seconds;

	if (index > DIGIT_COUNT - 1)
	{
		index = DIGIT_COUNT - 1;
	}

	seconds = (MAX_SEC - (int)pregameTimer.getSeconds());

	if (seconds < 0.5)
	{
		cdTimer.stop();
		state = PozaGame::GAME;
		cdTimer.start();
		//initHideAnimationParams();
	}

	currentDigit = digits[index];
}

void PozaGame::drawCircles()
{
	Vec2f initPosition = Vec2f(98.0f, 67.0f);
	Vec2f shift = Vec2f(195.0f, 0.0f);

	for (int i = 0; i < pozaNum - 1; i++)
	{
		gl::Texture krug = gameScore[i] ? good : bad;
		gl::draw(krug, Vec2i(controlsPos) + initPosition + shift * i);
	}	
}

void PozaGame::drawCounturLayer()
{
	gl::draw(poza.conturTex);
}

void PozaGame::drawCurrentPosePoints()
{
	gl::color(Color(1, 0, 0));

	for (size_t i = 0; i < humanPoints.size(); i++)
	{			
		gl::drawSolidCircle(humanPoints[i], 15.0f, 32);
	}
	gl::color(Color(1, 1, 1));

	gl::color(Color(0, 1, 1));
	for (size_t i = 0; i < poza.points.size(); i++)
	{
		gl::drawSolidCircle(poza.points[i], 25.0f, 32);
	}
	gl::color(Color(1, 1, 1));

	for (size_t i = 0; i < poza.points.size(); i++)
	{
		if (11 == i)
		{
			gl::color(Color(0, 1, 0));
		}
		gl::drawSolidCircle(poza.points[i], 15.0f, 32);
		gl::color(Color(1, 1, 1));
	}
}

void PozaGame::drawProgressBar()
{
	mathPercent = 0.4;
	gl::color(ColorA(1, 0, 0, 0.5f));
	gl::pushMatrices();
	gl::translate(controlsPos);
	gl::drawSolidRect(Rectf(0, 0, 1080 * mathPercent, 50));
	gl::popMatrices();
	gl::color(Color(1, 1, 1));	
}

void PozaGame::drawTimer()
{
	std::string sSeconds = seconds > 9 ? toString(seconds) : "0" + toString(seconds);

	auto digitTexture = textTools().getTextField("0:" + sSeconds, &numsFont, ci::Color::white());

	if (seconds <= 5)
	{
		gl::color(1, 0, 0);
	}
	else
	{
		gl::color(1, 1, 1);
	}
	gl::draw(digitTexture, Vec2i(controlsPos) + Vec2i(710, 40));
	gl::color(1, 1, 1);
}

void PozaGame::drawFailImage()
{
	gl::draw(fail);
}

void PozaGame::stopAllTweens()
{
	previewAnimateX.stop();
	alphaAnim.stop();
	IGameLocation::stopAllTweens();
}

void PozaGame::checkForPoseGuess()
{
	if (mathPercent > MATCHING_THRESHOLD)
	{
		levelCompletion++;
	}
	else if (--levelCompletion < 0)
	{
			levelCompletion = 0;
	}

	if (levelCompletion > LEVEL_COMPLETION)
	{
		state = SHOOTING;
		cdTimer.stop();
		cameraCanon().takePicture();	
	}
}
