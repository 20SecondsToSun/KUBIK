#pragma once
#include "IPhotoboothLocation.h"
#include "CameraAdapter.h"
#include "TextTools.h"
#include "DrawTools.h"
#include "shaders/ShaderTool.h"
#include "shaders/MaskShader.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef shared_ptr<class PhotoTimer> PhotoTimerRef;

			class PhotoTimer:public IPhotoboothLocation
			{	
				int MAX_SEC;
				int seconds;
				ci::Timer cdTimer;
				ci::gl::Texture title;
				ci::gl::Texture timerTex1, timerTex2;
				ci::Vec2f titlePos, timerTexPos;

				std::vector<ci::gl::Texture> digits;


				float startAngle, endAngle, changeAngle, RADIUS, rotor, centerY;

				shaders::imagefilters::MaskShaderRef maskShader;

				void drawAnimationCircle();

			public:
				void clear();

				PhotoTimer(PhotoboothSettingsRef settings);
				~PhotoTimer();

				virtual void reset(PhotoboothSettingsRef sett);			
				virtual void start();
				virtual void stop();
				virtual void update();
				virtual void draw();
			};		
		}
	}
}