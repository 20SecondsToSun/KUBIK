#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "MenuScreen.h"
#include "TuneUpScreen.h"
#include "ScreenSaver.h"
#include "ServicePopup.h"
#include "Types.h"
#include "ScreenSaverSettings.h"
#include "TuneUpSettings.h"
#include "KubikException.h"
#include "ApplicationView.h"
#include "ApplicationModel.h"
#include "GameSettings.h"

using namespace std;

namespace kubik
{
	class Controller
	{
	public:

		Controller(ApplicationView* view);
		void initLoad();

	private:

		ApplicationModel	*model;	
		ApplicationView		*view;

		Graphics			*graphicsLoader;

		MenuScreen			*menu;
		TuneUpScreen		*settings;
		GameScreen			*game;
		ScreenSaver			*screenSaver;
		Preloader			*preloader;
		ServicePopup		*servicePopup;

		MenuSettings		*menuSettings;
		ScreenSaverSettings *screenSaverSettings; 
		TuneUpSettings		*tuneUpSettings; 
		GameSettings		*gameSettings; 		

		vector<int>			changes;	

		void loadAllLocationsConfigs();

		void setConfigs();
		void setScreens();

		void loadGraphics();
		void allGraphicsLoadingCompleteHandler();
		void removeGraphicsLoadingSignals();
		void graphicsLoadErrorHandler(KubikException exc);

		void firstStart();

		////////////////////////////////////////////////////////////////////////////
		//
		//				SCREEN SAVER
		//
		////////////////////////////////////////////////////////////////////////////

		void startScreenSaver();
		void closeScreenSaverHandler();
		void startScreenSaverHandler();

		////////////////////////////////////////////////////////////////////////////
		//
		//					MENU SCREEN
		//
		////////////////////////////////////////////////////////////////////////////

		void startMenuScreen();
		void startMultiplyGameMode();

		////////////////////////////////////////////////////////////////////////////
		//
		//					SETTINGS SCREEN
		//
		////////////////////////////////////////////////////////////////////////////

		void startSettingsHandler();
		void startSettingsScreen();
		void closeSettingsHandler();
		void appSettingsChangedHandler(vector<int> changes);
		void reloadScreens(vector<int> changes);
		void allGraphicsReloadCompleteHandler();
		
		////////////////////////////////////////////////////////////////////////////
		//
		//					GAME SCREEN
		//
		////////////////////////////////////////////////////////////////////////////	

		void startGameHandler(int gameId);
		void createGame(int gameId);
		void closeGameHandler();
		void clearGameByID(int id);
		void gameGraphicsLoadingCompleteHandler();
		void resetGame();
		void startGame();		

		////////////////////////////////////////////////////////////////////////////
		//
		//					SERVICE POPUP SCREEN
		//
		////////////////////////////////////////////////////////////////////////////

		void servicePopupShow(KubikException exc);
	};
}