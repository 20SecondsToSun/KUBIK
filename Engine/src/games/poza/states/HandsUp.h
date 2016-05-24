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
			typedef std::shared_ptr<class HandsUp> HandsUpRef;

			class HandsUp :public IGameLocation
			{
				ci::gl::Texture titlte, man;
				ci::Vec2f titleTexPos, manTexPos;
				ci::Anim<float> alphaAnim;
				SimpleSpriteButtonRef voidBtn;

				float animTime;
				void hideAnimation(EventGUIRef& event);
				void hideAnimationComplete();
				void initAnimationcomplete();
				void disconnectAllListeners();
				void handsUpDetectionHandler(EventGUIRef& event);
				
			public:
				HandsUp(config::PozaSettingsRef settings);
				~HandsUp();
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