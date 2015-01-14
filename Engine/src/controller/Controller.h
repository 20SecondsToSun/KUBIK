#pragma once

#include "ConfigLoader.h"
#include "GraphicsLoader.h"

#include "MenuScreen.h"
#include "ConfigScreen.h"
#include "ScreenSaver.h"

#include "Types.h"

#include "ScreenSaverSettings.h"
#include "ConfigSettings.h"
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
		Controller(AppModelRef model, AppViewRef view);
		~Controller();

	private:
		AppModelRef				model;	
		AppViewRef				view;

		ConfigLoaderRef			configLoader;
		GraphicsLoaderRef		graphicsLoader;

		ScreenSaverSettingsRef	screenSaverSettings; 
		ConfigSettingsRef		controlSettings; 
		GameSettingsRef			gameSettings; 	
		MenuSettingsRef			menuSettings; 

		MenuScreenRef			menuScreen;
		ConfigScreenRef			controlScreen;		
		ScreenSaverRef			screenSaver;

		GamesFactory<IGame>				gamesFactory;
		GamesFactory<IGame>::base_ptr	game;

		IScreenRef currentLocation;			

		void loadKubikConfig();
		void kubikConfigLoadingCompleteHandler();
		void kubikConfigLoadingErrorHandler(KubikException exc);
		void removeKubikConfigLoadingConnections();	
		
		void loadSettings();
		void configsLoadingCompleteHandler();
		void configsLoadingErrorHandler(KubikException exc);
		void removeConfigsLoadingConnections();

		void loadAllLocationsGraphics();
		void allGraphicsLoadingCompleteHandler();
		void graphicsLoadingErrorHandler(KubikException exc);
		void removeGraphicsLoadingConnections();

		void createLocations();
		void startup();

		void startLocation(IScreenRef location);
		void closeCurrentLocation();

		void setLocationHandlers(ScreenId type);
		void removeLocationHandlers(ScreenId type);

		////////////////////////////////////////////////////////////////////////////
		//
		//				SCREEN SAVER
		//
		////////////////////////////////////////////////////////////////////////////

		void setScreenSaverHandlers();
		void removeScreenSaverHandlers();
	
		////////////////////////////////////////////////////////////////////////////
		//
		//					MENU SCREEN
		//
		////////////////////////////////////////////////////////////////////////////
		
		void setMenuScreenHandlers();
		void removeMenuScreenHandlers();
		void startMultiplyGameMode();

		////////////////////////////////////////////////////////////////////////////
		//
		//					CONFIG SCREEN
		//
		////////////////////////////////////////////////////////////////////////////

		void setConfigScreenHandlers();
		void removeConfigScreenHandlers();	
		void configScreenRemoveListeners();

		////////////////////////////////////////////////////////////////////////////
		//
		//					RELOAD KUBIK DATA	
		//
		////////////////////////////////////////////////////////////////////////////

		ISettingsRef getSettingsByChangeId(ChangeId id);
		IScreenRef getScreenByChangeId(ChangeId id);	

		void startAfterReload(GameId id = GameId::UNDEFINED);
		void appSettingsChangedHandler(vector<Changes> changes);
		void reloadScreens(vector<Changes> changes);
		void allGraphicsReloadCompleteHandler(vector<Changes> changes);	

		GameId getReloadGameId();

		////////////////////////////////////////////////////////////////////////////
		//
		//					GAME SCREEN
		//
		////////////////////////////////////////////////////////////////////////////
		
		void createGame(GameId id);
		void resetGame();
		void reloadGame(GameId id);

		void addGameHandlers();
		void removeGameHandlers();
		void startGameHandler(GameId id);		

		void closeGameHandler();
		void clearGameByID(GameId id);
		void gameGraphicsLoadingCompleteHandler();		
	};
}