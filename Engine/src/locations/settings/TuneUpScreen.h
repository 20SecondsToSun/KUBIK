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
#include "ScreenSaverSettings.h"
#include "Types.h"

#include "cinder/params/Params.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class TuneUpScreen:public IScreen
	{
	public:

		TuneUpScreen(shared_ptr<TuneUpSettings> screenSaverSettings,
					 shared_ptr<ScreenSaverSettings>      config,
					 shared_ptr<MenuSettings>        menuConfig,
					 shared_ptr<GameSettings>		 gameSettings);

		~TuneUpScreen();

		void draw();	
		void init(shared_ptr<ISettings> settings) override;
		
		signal<void(void)> closeSettingsSignal;
		signal<void(vector<Changes>)> appSettingsChangedSignal;

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

		shared_ptr<TuneUpSettings>		tuneUpSettings;
		shared_ptr<ScreenSaverSettings> screnSaversettings;
		shared_ptr<MenuSettings>		menuConfig;
		shared_ptr<GameSettings>		gameSettings;

		ButtonText*	saveChngBtn;
		Button*		closeBtn;

		void closeLocationHandler(Button& button);
		void appSettingsChgHandler(ButtonText& button);

		vector<Changes> changes;
		
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