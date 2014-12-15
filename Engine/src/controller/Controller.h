#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "MenuScreen.h"
#include "TuneUpScreen.h"
#include "ScreenSaver.h"
#include "ServicePopup.h"
#include "Preloader.h"
#include "Types.h"
#include "ScreenSaverSettings.h"
#include "TuneUpSettings.h"
#include "KubikException.h"
#include "ApplicationView.h"
#include "ApplicationModel.h"
#include "GameSettings.h"
#include "GamesFactory.h"
#include "Photobooth.h"
#include "Funces.h"

using namespace std;

namespace kubik
{
	class Controller
	{
	public:

		Controller(shared_ptr<ApplicationView> view);
		~Controller();
		void initLoad();

	private:

		struct LocMapper
		{
			shared_ptr<IScreen> screen;
			shared_ptr<ISettings> settings;
		};	

		shared_ptr<ApplicationModel>	model;	
		shared_ptr<ApplicationView>		view;

		shared_ptr<Graphics>			graphicsLoader;

		shared_ptr<MenuScreen>			menu;
		shared_ptr<TuneUpScreen>		settings;		
		shared_ptr<ScreenSaver>			screenSaver;
		shared_ptr<Preloader>			preloader;
		shared_ptr<ServicePopup>		servicePopup;

		shared_ptr<ScreenSaverSettings>	screenSaverSettings; 
		shared_ptr<TuneUpSettings>		tuneUpSettings; 
		shared_ptr<GameSettings>		gameSettings; 	
		shared_ptr<MenuSettings>		menuSettings; 		

		GamesFactory<IGame>				gamesFactory;
		GamesFactory<IGame>::base_ptr	game;		

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
		void appSettingsChangedHandler(vector<Changes> changes);
		void reloadScreens(vector<Changes> changes);
		void allGraphicsReloadCompleteHandler();
		void settingsScreenRemoveListeners();

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


		////////////////////////////////////////////////////////////////////////////
		//
		//				
		//
		////////////////////////////////////////////////////////////////////////////

		LocMapper getLocationPair(int id);
		vector<Changes> reloadSettingsChanges;

		bool reloadOneGame;

		int reloadOneGameId;
	};
}