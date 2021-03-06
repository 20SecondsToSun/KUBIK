#pragma once

#include "main/IGameLocation.h"
#include "PhotoboothSettings.h"
#include "CameraAdapter.h"
#include "DrawTools.h"
#include "TimerTools.h"
#include "main/PhotoStorage.h"
#include "shaders/ShaderTool.h"
#include "states/timer/PhotoTimer.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef std::shared_ptr<class PhotoShooting> PhotoShootingRef;

			class PhotoShooting : public IGameLocation
			{
				static const int SHOT_NUM = 5;

				enum stateID
				{
					PEPARE_FOR_SHOOTING,
					START_ANIM,
					LIVE_VIEW,
					SHOOTING,
					PREVIEW
				}
				state;

				size_t	currentShot, shotsNum, smileIndex;
				bool checkTimerInUpdate;
				float delayShootingTime, liveViewPrepareTime, previewShowingTime;
				float progressBlockStartY;
				float cameraScale, cameraWidth, cameraHeight;
				float maskSizeInit, maskSizeFinal;
				shaders::imagefilters::BaseShaderRef shader;
				shaders::imagefilters::MaskShaderRef maskShader;

				PhotoStorageRef photoStorage;
				ci::gl::Texture countsTex, seekTex, photo, line, frame, backgroundProgresstexture, shadow;
				ci::gl::Texture cameraTexture, photoTemplate;
				ci::Vec2f countsTexPos, seekTexPos0, framePosition;
				ci::Vec2f cameraPosition;

				std::vector<ci::gl::Texture> smileTexs;

				ci::Anim<float> percent, progressBlockAnimateY, previewAnimateX;
				ci::Anim<ci::Vec2f> seekPosition;
				ci::Anim<float> maskSize;

				void callDelayShotTimer();
				void callPreviewShowingTimer();
				void callLiveViewPrepareTimer();

				void photoTakenHandler();
				void photoDownloadHandler(const std::string& path);
				void photoErrorHandler();

				void delayshoot();
				void previewdelay();
				void liveviewdelay();

				void drawProgressBlock();
				void drawDashedFrame();
				void drawPhotoMaskIntro();				
				void showAnimationComplete();
				void drawCameraTexture();
				void drawSmile();
				void drawPhotoframe();
				int getNextSmileIndex();

				ci::signals::connection photoTakenCon, photoErrorCon, photoDownloadedCon;

			public:
				PhotoShooting(config::PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage);
				~PhotoShooting();

				virtual void reset(config::ISettingsRef settings) override;
				virtual void start() override;
				virtual void update() override;
				virtual void draw() override;
				virtual void stop() override;
				virtual void stopAllTweens() override;
			};
		}
	}
}
