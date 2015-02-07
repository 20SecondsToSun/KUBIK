#pragma once
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "IScreen.h"
#include "ConfigSettings.h"
#include "MenuSettings.h"
#include "Button.h"
#include "ButtonText.h"
#include "GameSettings.h"
#include "ScreenSaverSettings.h"
#include "Types.h"
#include "IDispatcher.h"
#include "CloseConfigEvent.h"

#include "main/MainConfig.h"
#include "photobooth/PhotoboothConfig.h"
#include "gui/CompositeDispatcher.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace params;
using namespace kubik::config;

namespace kubik
{
	class ConfigScreen:public IScreen, public CompositeDispatcher
	{
	public:
		ConfigScreen(ISettingsRef config);
		~ConfigScreen();

		void start();
		void stop();
		void draw();	
		void init();
		void init(ISettingsRef settings) override;

		void reset() override{};
		void startUpParams();
		void savePhtbtn();
		
		SignalVoid closeSettingsSignal;
		signal<void(vector<Changes>)> appSettingsChangedSignal;

		connection mouseUpListener;
		connection closeBtnListener;
		connection appSettingsChgListener;

		void setScreenSaverSettings(ScreenSaverSettingsRef screenSaverSettings);
		void setMenuSettings(MenuSettingsRef menuSettings);
		void setGameSettings(GameSettingsRef gameSettings);

		//void activateListeners();
		//void unActivateListeners();
	
	private:
		ConfigSettingsRef		configSettings;
		ScreenSaverSettingsRef  screenSaverSettings;
		MenuSettingsRef			menuSettings;
		GameSettingsRef			gameSettings;

		ButtonTextRef			saveChngBtn;
		ButtonRef				closeBtn;
	
		vector<Changes> changes;

		//PhotoboothSettings::PhotoboothDataStruct phData, initPhData;
		MenuSettings::MenuDataStruct menuData, initialMenuData;		
		GameSettings::GamesDataStruct gamesData, initialGamesData;	
		ScreenSaverSettings::ScreenSaverDataStruct screensaverData, initialScreensaverData;	

		MainConfigRef mainConfig; 
		PhotoboothConfigRef photoboothConfig;

		void update();
		void closeLocationHandler(EventRef& event);
		void gamesBlockHandler(EventGUIRef& event);

		void appSettingsChgHandler(ButtonText& button);		
		
		void savePhotoboothParams();

		void createPhotoboothParams();
		void createMenuParams();	
		void createFuncesParams();	

		void checkPhotoBoothParamsForChanges();
		void checkFuncesParamsForChanges();
		void checkMenuParamsForChanges();
		void checkGamesParamsForChanges();
		void checkScreenSaverParamsForChanges();
		void createGamesParams();		
		void createScreensaverParams();

		void showInExplorer(string path);
		void showInExplorerMenuDesignPath();
		
		void setDefaultGameIdInSwitchOnGames();
		void setReloadGamePropertyIfNeedIt(Changes &chng);
		
	};

	typedef shared_ptr<ConfigScreen> ConfigScreenRef;	
}