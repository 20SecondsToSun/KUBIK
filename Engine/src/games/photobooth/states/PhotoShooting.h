#pragma once

#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "CameraAdapter.h"
#include "DrawTools.h"
#include "model/PhotoStorage.h"
#include "shaders/ShaderTool.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			class PhotoShooting: public IPhotoboothLocation
			{
				static const int SHOT_NUM = 5;

				int	currentShot, shotsNum;
				int	secBetweenShots;
				bool checkTimerInUpdate;
				float delayShootingTime, liveViewPrepareTime, previewShowingTime;
				float progressBlockStartY;

				shaders::imagefilters::BaseShaderRef shader;

				PhotoStorageRef photoStorage;
				ci::gl::Texture countsTex, seekTex, photo, smileTex, line, frame;				
				ci::Vec2f countsTexPos, seekTexPos, seekTexPos0, framePosition;		
				ci::Anim<float> _time, percent, alphaPreview;	
				ci::Anim<ci::Vec2f> seekPosition;

				float cameraScale, cameraWidth,	cameraHeight;
				ci::Vec2f cameraPosition;

				ci::gl::Texture cameraTexture, photoTemplate;

				enum stateID
				{
					PEPARE_FOR_SHOOTING,
					LIVE_VIEW,
					SHOOTING,
					PREVIEW
				}
				state;

				void callDelayShotTimer();
				void callPreviewShowingTimer();
				void callLiveViewPrepareTimer();

				void photoTakenHandler();
				void photoDownloadHandler(const string& path);
				void photoErrorHandler();

				void delayshoot();
				void previewdelay();	
				void liveviewdelay();

				void drawProgressBlock();
				void drawDashedFrame();

			public:
				PhotoShooting(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage);
				void start();
				void update();
				void draw();		
				void reset(PhotoboothSettingsRef settings);		
			};

			typedef shared_ptr<PhotoShooting>	 PhotoShootingRef;
		}
	}
}