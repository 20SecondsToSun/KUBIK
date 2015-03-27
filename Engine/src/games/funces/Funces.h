#pragma once

#include "ApplicationModel.h"
#include "IGame.h"
#include "ISettings.h"
#include "FuncesSettings.h"
#include "gui/Sprite.h"

namespace kubik
{
	namespace games
	{
		namespace funces
		{
			class Funces :public IGame
			{
			public:
				Funces(ISettingsRef setRef);
				~Funces();

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
				FuncesSettingsRef settings;

				void closeMouseUpHandler(IButton& button);
			};
		}
	}
}