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
			typedef std::shared_ptr<class PozaInstruction> PozaInstructionRef;

			class PozaInstruction :public IGameLocation
			{
				ci::gl::Texture fonTex;
				ci::Vec2f titleTexPos;
				ci::Anim<float> alphaAnim;
				SimpleSpriteButtonRef voidBtn;

				float animTime;
				void hideAnimation(EventGUIRef& event);
				void hideAnimationComplete();
				void initAnimationcomplete();

			public:
				PozaInstruction(config::PozaSettingsRef settings);
				~PozaInstruction();
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