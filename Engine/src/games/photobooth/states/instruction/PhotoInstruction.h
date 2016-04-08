#pragma once
#include "states/IPhotoboothLocation.h"
#include "gui/SimpleSpriteButton.h"
#include "PhotoboothSettings.h"
#include "TimerTools.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef std::shared_ptr<class PhotoInstruction> PhotoInstructionRef;

			class PhotoInstruction :public IPhotoboothLocation
			{
				ci::gl::Texture fonTex, titleTex;
				ci::Vec2f titleTexPos;
				ci::Anim<float> alphaAnim;
				SimpleSpriteButtonRef voidBtn;

				float animTime;
				void hideAnimation(EventGUIRef& event);
				void hideAnimationComplete();
				void initAnimationcomplete();
				
			public:
				PhotoInstruction(config::PhotoboothSettingsRef settings);
				~PhotoInstruction();
				virtual void reset(config::PhotoboothSettingsRef set) override;
				virtual void start() override;
				virtual void stop() override;
				virtual void update() override;
				virtual void draw() override;
				virtual void stopAllTweens() override;
			};
		}
	}
}