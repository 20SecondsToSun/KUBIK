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
		typedef shared_ptr<class PhotoTimer> PhotoTimerRef;

		class PhotoTimer:public IPhotoboothLocation
		{	
			int MAX_SEC;
			int seconds;
			ci::Timer cdTimer;
			ci::gl::Texture tex1, tex2;
			ci::gl::Texture timerTex1, timerTex2;
			ci::Vec2f title1Pos, title2Pos, timerTexPos;
			ci::Font timerFont;		

			float startAngle, endAngle, changeAngle, RADIUS, rotor;

			shaders::imagefilters::MaskShaderRef maskShader;

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