#pragma once

#include "ApplicationModel.h"
#include "IGame.h"
#include "ISettings.h"
#include "FuncesSettings.h"
#include "gui/Sprite.h"
#include "Kinect2.h"
#include "kinect2/KinectAdapter.h"

namespace kubik
{
	namespace games
	{
		namespace funces
		{
			class Funces :public IGame
			{
			public:
				Funces(config::ISettingsRef setRef);
				~Funces();

				void start();
				void initKinect();				
				void draw();
				void update(){};

				virtual void stop() override;
				virtual void reset() override;
				virtual void init(config::ISettingsRef config) override;
				virtual void showAnimationComplete() override;

			private:
				ci::gl::Texture closeImg;
				ci::signals::connection mouseUpListener, closeBtnListener;
				config::FuncesSettingsRef settings;
				
				Kinect2::BodyFrame			mBodyFrame;

				ci::Channel8u				mChannelBodyIndex;
				ci::Channel16u				mChannelDepth;
				ci::Surface8u				mSurfaceColor;

				void closeMouseUpHandler(IButton& button);
			};
		}
	}
}
