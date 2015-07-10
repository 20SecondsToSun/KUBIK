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
				Poza(ISettingsRef setRef);
				~Poza();

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
				PozaSettingsRef settings;

				void closeMouseUpHandler(IButton& button);
			};
		}
	}
}