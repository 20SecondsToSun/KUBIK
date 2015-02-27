#pragma once

#include "ApplicationModel.h"
#include "IGame.h"
#include "ISettings.h"
#include "FuncesSettings.h"
#include "gui/Sprite.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class Funces:public IGame, public Sprite
	{
	public:
		Funces(ISettingsRef setRef);
		~Funces();

		void start();
		void stop() override;

		void draw();	
		void reset() override;
		void init(ISettingsRef config) override;

	private:
		Texture closeImg;
		//ButtonRef closeBtn;
		connection mouseUpListener, closeBtnListener;	
		FuncesSettingsRef settings;

		void closeMouseUpHandler(IButton& button);		
	};
}