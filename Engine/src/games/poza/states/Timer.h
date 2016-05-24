#pragma once
#include "gui/SimpleSpriteButton.h"
#include "PozaSettings.h"
#include "TimerTools.h"
#include "main/IGameLocation.h"

namespace kubik
{
	namespace games
	{
		namespace poza
		{
			typedef std::shared_ptr<class PozaTimer1> PozaTimer1Ref;

			class PozaTimer1 :public IGameLocation
			{
				static const int DIGIT_COUNT = 4;
				int MAX_SEC;
				static const int centerY = 1000.0f;

				std::vector<ci::gl::Texture> digits;

				ci::gl::Texture title2, circles, currentDigit;
				ci::Vec2f titleTexPos, circlesTexPos;
				ci::Anim<float> alphaAnim, digitScale;
				ci::Timer cdTimer;
				int seconds;
				

				float animTime;
				void hideAnimation(EventGUIRef& event);
				void hideAnimationComplete();
				void initAnimationcomplete();
				void calculateDigit();
				void drawDigit();
				void initHideAnimationParams();

				enum TimerStates
				{
					SHOW_ANIM,
					COUNTING,
					HIDE_ANIM
				}state;

			public:
				PozaTimer1(config::PozaSettingsRef settings);
				~PozaTimer1();
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