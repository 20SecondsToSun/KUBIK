#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IScreen.h"
#include "TuneUpSettings.h"
#include "MenuSettings.h"
#include "Button.h"
#include "GameSettings.h"

#include "cinder/params/Params.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class TuneUpScreen:public IScreen
	{
	public:

		TuneUpScreen(TuneUpSettings* config, MenuSettings* menuConfig, GameSettings* gameSettings);
		~TuneUpScreen();

		void draw();	
		void init(TuneUpSettings* settings);
		
		signal<void(int)>  startGameSignal;	
		signal<void(void)> closeSettingsSignal;
		signal<void(vector<int>)> appSettingsChangedSignal;

		void addMouseUpListener();
		void removeMouseUpListener();

		ci::signals::connection mouseUpListener, closeBtnListener, appSettingsChgListener;	

		void startUpParams();
		void savePhtbtn();

	private:

		params::InterfaceGlRef	photoBoothParams, menuParams;


		void update();
		void mouseUp(MouseEvent &event);

		Font font;
		TuneUpSettings* settings;
		MenuSettings* menuConfig;
		GameSettings* gameSettings;

		ButtonText *saveChngBtn;
		shared_ptr<Button> closeBtn;

		void closeLocationHandler(Button& button);
		void appSettingsChgHandler(ButtonText& button);

		vector<int> changes;
		
		void savePhotoboothParams();

		void createPhotoboothParams();
		void createMenuParams();
		
		struct
		{
			bool isFacebook;
			bool isVkotakte;
			bool isTwitter;
			bool isCustomDesign;
			bool isPrinter;
			bool isEmail;
			bool isQrCode;
			int  seconds;
			int  secondsBetweenShots;
			int  photoNum;
		}phSet;
	};
}