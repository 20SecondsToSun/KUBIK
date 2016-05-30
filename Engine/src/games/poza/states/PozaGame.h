#pragma once

#include "gui/SimpleSpriteButton.h"
#include "PozaSettings.h"
#include "TimerTools.h"
#include "CameraAdapter.h"
#include "main/PhotoStorage.h"
#include "model/PozaBase.h"
#include "model/HumanModel.h"
#include "main/IGameLocation.h"
#include "kinect2/KinectAdapter.h"

namespace kubik
{
	namespace games
	{
		namespace poza
		{
			typedef std::shared_ptr<class PozaGame> PozaGameRef;

			class PozaGame :public IGameLocation
			{
				const float kinectShiftY = -660;

				ci::gl::Texture controls, fail, good, bad, returnTitle, circles, currentDigit;
				ci::gl::Texture currentCardTemplate;

				ci::Vec2f controlsPos, titleTexPos, circlesTexPos;
				ci::Anim<float> alphaAnim;
				ci::Anim<float> previewAnimateX;				
				std::vector<ci::gl::Texture> digits;
				float animTime;

				void hideAnimation(EventGUIRef& event);
				void hideAnimationComplete();
				void initAnimationcomplete();
				void matchPozaTemplate();
				void updateJointsPosition();

				int MAX_SEC, DIGIT_COUNT;
				int pozaNum;
				int onePozaTime;
				int seconds;

				float cameraScale, cameraWidth, cameraHeight;				

				std::vector<int>& gameScore;
				ci::Font numsFont;
				ci::Timer cdTimer, pregameTimer;				
				ci::Vec2f cameraPosition;
				ci::gl::Texture cameraTexture;
				ci::gl::Texture photo;

				PozaBase pozaData;
				std::shared_ptr<HumanModel> humanModel;
				float mathPercent;
				std::vector<ci::Vec2f> humanPoints;

				enum GameStates
				{
					STARTING,
					GAME,
					PREGAME,
					FAIL,
					SUCCESS,
					HIDE_ANIM,
					SHOOTING,
					PREVIEW
				}
				state;

				ci::signals::connection photoTakenCon, photoErrorCon, photoDownloadedCon;
				SimpleSpriteButtonRef voidBtn;
				photobooth::PhotoStorageRef photoStorage;
				kubik::config::PozaSettings::PozaDataObj poza;

				Kinect2::BodyFrame			mBodyFrame;
				ci::Channel8u				mChannelBodyIndex;
				ci::Channel16u				mChannelDepth;
				ci::Surface8u				mSurfaceColor;

				int colorPoint = 0;
				int levelCompletion = 0;
				static const int LEVEL_COMPLETION = 10;
				const float MATCHING_THRESHOLD = 0.8;
				
				void drawCircles();
				void drawCameraLayer();
				void drawKinectStream();				
				void drawCounturLayer();
				void drawCurrentPosePoints();				
				void drawProgressBar();
				void drawTimer();
				void drawFailImage();
				void drawPhotoframe();				
				void roundFinished(bool isGood = false);
				void newRoundInit();
				void calculateDigit();	
				void previewDelayHandler();
				void cardAnimationOutHandler();
				void checkForPoseGuess();

				void photoTakenHandler();
				void photoDownloadHandler(const std::string& path);
				void photoErrorHandler();
				void pozaSuccessHandler(EventGUIRef& event);

			public:
				PozaGame(config::PozaSettingsRef settings, kubik::games::photobooth::PhotoStorageRef  photoStorage, std::vector<int>& gameScore, PozaBase& pozaData, std::shared_ptr<HumanModel> humanModel);
				~PozaGame();
				virtual void reset(config::ISettingsRef set) override;
				virtual void start() override;
				virtual void stop() override;
				virtual void update() override;
				virtual void draw() override;
				virtual void stopAllTweens() override;
			};
		}
	}
}