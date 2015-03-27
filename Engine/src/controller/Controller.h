#pragma once

#include "ConfigLoader.h"
#include "GraphicsLoader.h"

#include "MenuScreen.h"
#include "ConfigScreen.h"
#include "ScreenSaver.h"
#include "ControlLayer.h"

#include "Types.h"

#include "ScreenSaverSettings.h"
#include "ConfigSettings.h"
#include "KubikException.h"
#include "ApplicationView.h"
#include "ApplicationModel.h"
#include "GameSettings.h"
#include "GamesFactory.h"
#include "Photobooth.h"
#include "instakub/Instakub.h"
#include "Funces.h"

#include "VirtualKeyboard.h"
#include "preloader/Preloader.h"

using namespace kubik::menu;
using namespace kubik::config;

namespace kubik
{
	class Controller
	{
	public:
		Controller(ApplicationModelRef model, AppViewRef view);
		~Controller();

	private:
		ApplicationModelRef		model;
		AppViewRef				view;

		ConfigLoaderRef			configLoader;
		GraphicsLoaderRef		graphicsLoader;

		ScreenSaverSettingsRef	screenSaverSettings; 
		ConfigSettingsRef		controlSettings; 
		GameSettingsRef			gameSettings; 	
		MenuSettingsRef			menuSettings;

		MenuScreenRef			menuScreen;
		ControlLayerRef			controlLayer;
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
		
		void startMenuScreenLocation();
		void removeMenuScreenHandlers();
		void setMenuScreenHandlers();
		
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

		void reloadScreens(const std::vector<Changes>& changes, bool reloadAllDesign);
	
		////////////////////////////////////////////////////////////////////////////
		//
		//					GAME SCREEN
		//
		////////////////////////////////////////////////////////////////////////////
		
		void createGame(GameId id);
		void resetGame();
		void reloadGame(GameId id);

		void addGameHandlers(); 
		void enableGameHandler();
		void disableGameHandler();
		void removeGameHandlers();
		void startGameHandler(GameId id);		

		void closeGameHandler();
		void openSettingsHandler();
		
		void clearGameByID(GameId id);
		void gameGraphicsLoadingCompleteHandler();		
	};
}