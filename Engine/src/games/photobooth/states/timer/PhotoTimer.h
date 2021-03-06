#pragma once
#include "main/IGameLocation.h"
#include "PhotoboothSettings.h"
#include "CameraAdapter.h"
#include "TextTools.h"
#include "DrawTools.h"
#include "shaders/ShaderTool.h"
#include "shaders/MaskShader.h"
#include "main/PhotoStorage.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef std::shared_ptr<class PhotoTimer> PhotoTimerRef;

			class PhotoTimer :public IGameLocation
			{
				static const int DIGIT_COUNT = 4;

				PhotoStorageRef photoStorage;
				
				int MAX_SEC;
				int seconds;
				float startAngle, endAngle, changeAngle, RADIUS, rotor;

				ci::Timer cdTimer;
				ci::gl::Texture timerTex1, timerTex2, currentDigit;
				ci::Vec2f titlePos, timerTexPos1, timerTexPos2;
				std::vector<ci::gl::Texture> digits;

				ci::Anim<float> circleScale, digitScale;

				shaders::imagefilters::MaskShaderRef maskShader;

				void drawAnimationCircle();
				void drawDigit();

				void initShowAnimationParams();
				void initHideAnimationParams();

				void showAnimationComplete();
				void hideAnimationComplete();

				void calculateDigit();

				enum TimerStates
				{
					SHOW_ANIM,
					COUNTING,
					HIDE_ANIM
				}state;

			public:
				PhotoTimer(config::PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage);
				virtual ~PhotoTimer();

				static const int centerY = 1123.0f;

				virtual void reset(config::ISettingsRef sett) override;
				virtual void start() override;
				virtual void stop() override;
				virtual void update() override;
				virtual void draw() override;
				virtual void stopAllTweens() override;
				void clear();
			};
		}
	}
}