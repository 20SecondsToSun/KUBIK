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
			typedef std::shared_ptr<class ThreeMeters> ThreeMetersRef;

			class ThreeMeters :public IGameLocation
			{
				ci::gl::Texture thrimeters, arrow;
				ci::Vec2f titleTexPos, arrowTexPos;
				ci::Anim<float> alphaAnim;
				SimpleSpriteButtonRef voidBtn;

				float animTime;
				void hideAnimation(EventGUIRef& event);
				void hideAnimationComplete();
				void initAnimationcomplete();

			public:
				ThreeMeters(config::PozaSettingsRef settings);
				~ThreeMeters();
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