#pragma once

#include "ApplicationModel.h"
#include "IGame.h"
#include "ISettings.h"
#include "PozaSettings.h"
#include "gui/Sprite.h"

namespace kubik
{
	namespace games
	{
		namespace poza
		{
			class Poza :public IGame
			{
			public:
				Poza(config::ISettingsRef setRef);
				~Poza();

				void start();
				void draw();
				void update(){};

				virtual void stop() override;
				virtual void reset() override;
				virtual void init(config::ISettingsRef config) override;
				virtual void showAnimationComplete() override;

			private:
				ci::gl::Texture closeImg;
				ci::signals::connection mouseUpListener, closeBtnListener;
				config::PozaSettingsRef settings;

				void closeMouseUpHandler(IButton& button);
			};
		}
	}
}