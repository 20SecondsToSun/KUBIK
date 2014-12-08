#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IScreen.h"
#include "TuneUpSettings.h"
#include "MenuSettings.h"
#include "Button.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class TuneUpScreen:public IScreen
	{
	public:

		TuneUpScreen(TuneUpSettings* config, MenuSettings* menuConfig);
		~TuneUpScreen();

		signal<void(int)> startGameSignal;	

		void draw();	
		void init();
		
		signal<void(void)> closeSettingsSignal;
		signal<void(vector<SettingTypes>)> appSettingsChangedSignal;

		void addMouseUpListener();
		void removeMouseUpListener();

		ci::signals::connection mouseUpListener, closeBtnListener, appSettingsChgListener;	

	protected:
		void setTextures();

	private:
		void update();
		void mouseUp(MouseEvent &event);

		Font font;
		TuneUpSettings* settings;
		MenuSettings* menuConfig;

		ButtonText *menuDesignChngBtn;
		shared_ptr<Button> closeBtn;

		void closeLocationHandler(Button& button);
		void appSettingsChgHandler(ButtonText& button);
	};
}