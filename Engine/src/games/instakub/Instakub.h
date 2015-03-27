#pragma once

#include "ApplicationModel.h"
#include "IGame.h"
#include "ISettings.h"
#include "InstakubSettings.h"
#include "gui/Sprite.h"

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			class Instakub :public IGame
			{
			public:
				Instakub(ISettingsRef setRef);
				~Instakub();				
				
				void start();
				void draw();
				void update(){};

				virtual void stop() override;
				virtual void reset() override;
				virtual void init(ISettingsRef config) override;
				virtual void showAnimationComplete() override;

			private:
				ci::gl::Texture closeImg;
				connection mouseUpListener, closeBtnListener;
				InstakubSettingsRef settings;

				void closeMouseUpHandler(IButton& button);
			};
		}
	}
}