#pragma once

#include "gui/SimpleSpriteButton.h"
#include "PozaSettings.h"
#include "TimerTools.h"
#include "main/IGameLocation.h"
#include "kinect2/KinectAdapter.h"
#include "games/poza/model/HumanModel.h"

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
				std::shared_ptr<HumanModel> humanModel;

				float animTime;
				bool detected;

				void hideAnimation(EventGUIRef& event);
				void hideAnimationComplete();
				void initAnimationcomplete();
				void disconnectAllListeners();
				void handsUpDetectionHandler();

				Kinect2::BodyFrame			mBodyFrame;
				ci::Channel8u				mChannelBodyIndex;
				ci::Channel16u				mChannelDepth;
				KinectAdapter::BodyFilter	centerBody;
				
			public:
				HandsUp(config::PozaSettingsRef settings, std::shared_ptr<HumanModel> humanModel);
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