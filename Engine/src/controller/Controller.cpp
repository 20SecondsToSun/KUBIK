#include "Controller.h"

using namespace kubik;
using namespace kubik::menu;
using namespace kubik::games;
using namespace std;

Controller::Controller(ApplicationModelRef model, AppViewRef view)
{
	this->view    = view;
	this->model   = model;
	game		  = NULL;

	view->showPreloader();

	loadKubikConfig();
	loadSettings();
	loadAllLocationsGraphics();
}

////////////////////////////////////////////////////////////////////////////
//
//				LOADING KUBIK CONFIG
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadKubikConfig()
{
	configLoader = ConfigLoaderRef(new ConfigLoader(model));
	configLoader->load();	
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATE SETTINGS
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadSettings()
{
	logger().log("START LOADING OTHER CONFIGS");

	controlSettings		 = ConfigSettingsRef(new ConfigSettings(model));
	gameSettings		 = GameSettingsRef(new GameSettings(model, controlSettings));
	menuSettings		 = MenuSettingsRef(new MenuSettings(model));	
	screenSaverSettings  = ScreenSaverSettingsRef(new ScreenSaverSettings(model));
	keyboardSettings	 = KeyboardSettingsRef(new KeyboardSettings(model));
	socialSettings		 = SocialSettingsRef(new SocialSettings(model));
	preloaderSettings	 = PreloaderSettingsRef(new PreloaderSettings(model, controlSettings));

	settingsFactory().inject(controlSettings);
	settingsFactory().inject(preloaderSettings);
	settingsFactory().inject(screenSaverSettings);

	list<ISettingsRef> configs;
	configs.push_back(controlSettings);
	configs.push_back(gameSettings);
	configs.push_back(menuSettings);	
	configs.push_back(screenSaverSettings);
	configs.push_back(keyboardSettings);
	configs.push_back(socialSettings);
	configs.push_back(preloaderSettings);
	configLoader->loadConfigs(configs);

	logger().log("CONFIGS LOADED");

	PhotoboothSettingsRef phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(GameId::PHOTOBOOTH));
	settingsFactory().inject(phbthSettings);	
}

////////////////////////////////////////////////////////////////////////////
//
//				LOADING GRAPHICS
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadAllLocationsGraphics()
{	
	logger().log("START LOADING GRAPHICS");

	graphicsLoader = GraphicsLoaderRef(new GraphicsLoader());

	connect_once(graphicsLoader->LoadingCompleteSignal, bind(&Controller::allGraphicsLoadingCompleteHandler, this));
	connect_once(graphicsLoader->LoadingErrorSignal, bind(&Controller::graphicsLoadingErrorHandler, this, std::placeholders::_1));

	graphicsLoader->setLoadingTextures(keyboardSettings->getResources());
	graphicsLoader->setLoadingTextures(menuSettings->getResources());
	graphicsLoader->setLoadingTextures(screenSaverSettings->getResources());
	graphicsLoader->setLoadingTextures(controlSettings->getResources());
	graphicsLoader->setLoadingTextures(preloaderSettings->getResources());

	graphicsLoader->setLoadingTextures(gameSettings->getActiveGameTextures());
	graphicsLoader->setLoadingTextures(gameSettings->getGameSettingsTextures());	
	graphicsLoader->load();
}

void Controller::allGraphicsLoadingCompleteHandler()
{
	logger().log("BUILD SETTINGS DATA");

	preloaderSettings->buildData();
	controlSettings->buildData();
	gameSettings->buildSettingData();

	removeGraphicsLoadingConnections();
	createLocations();
}

void Controller::graphicsLoadingErrorHandler(KubikException exc)
{
	removeGraphicsLoadingConnections();
	view->showServicePopup(exc);
}

void Controller::removeGraphicsLoadingConnections()
{
	graphicsLoader->LoadingCompleteSignal.disconnect_all_slots();	
	graphicsLoader->LoadingErrorSignal.disconnect_all_slots();			
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATE LOCATIONS
//
////////////////////////////////////////////////////////////////////////////

void Controller::createLocations()
{	
	logger().log("CREATE LOCATIONS");

	touchKeyboard().create(keyboardSettings);

	screenSaver   = ScreenSaverRef(new ScreenSaver(screenSaverSettings));
	menuScreen	  = MenuScreenRef(new MenuScreen(menuSettings));	
	controlLayer  = ControlLayerRef(new ControlLayer(menuSettings));
	controlScreen = ConfigScreenRef(new ConfigScreen(controlSettings));

	controlScreen->setScreenSaverSettings(screenSaverSettings);
	controlScreen->setMenuSettings(menuSettings);
	controlScreen->setGameSettings(gameSettings);
	controlScreen->init();

	gameSettings->gamesfactoryReg();	
	
	createGame(model->getDefaultGameID());
	view->addLayer(controlLayer);

	//startup();
	startMenuScreenLocation();
}

////////////////////////////////////////////////////////////////////////////
//
//				STARTUP
//
////////////////////////////////////////////////////////////////////////////

void Controller::startup()
{
	if(screenSaverSettings->isShow())	
		startLocation(screenSaver);	
	else
		startMenuScreenLocation();
}

void Controller::startLocation(IScreenRef location)
{
	if (currentLocation)
		currentLocation->setScrenshot();

	closeCurrentLocation();

	currentLocation = location;

	setLocationHandlers(currentLocation->getType());

	currentLocation->start();
	view->startLocation(currentLocation);
}

void Controller::closeCurrentLocation()
{
	if(currentLocation)
	{
		currentLocation->stop();
		removeLocationHandlers(currentLocation->getType());	
	}
}

void Controller::setLocationHandlers(ScreenId type)
{
	if(gameSettings->isGameID(type))
		addGameHandlers();
	else
	{
		switch(type)
		{	
		case ScreenId::SCREENSAVER:
			setScreenSaverHandlers();
			break;

		case ScreenId::MENU:
			setMenuScreenHandlers();
			break;

		case ScreenId::CONFIG:
			setConfigScreenHandlers();
			break;		
		}
	}
}

void Controller::removeLocationHandlers(ScreenId type)
{
	if(gameSettings->isGameID(type))
		removeGameHandlers();
	else
	{
		switch(type)
		{
		case ScreenId::SCREENSAVER:
			removeScreenSaverHandlers();
			break;

		case ScreenId::MENU:
			removeMenuScreenHandlers();
			break;

		case ScreenId::CONFIG:
			removeConfigScreenHandlers();
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//
//				SCREEN SAVER
//
////////////////////////////////////////////////////////////////////////////

void Controller::setScreenSaverHandlers()
{
	connect_once(currentLocation->closeLocationSignal, bind(&Controller::startMenuScreenLocation, this));//, menuScreen));
}

void Controller::removeScreenSaverHandlers()
{
	screenSaver->closeLocationSignal.disconnect_all_slots();
}

////////////////////////////////////////////////////////////////////////////
//
//					MENU SCREEN
//
////////////////////////////////////////////////////////////////////////////

void Controller::startMenuScreenLocation()
{
	if (model->onlyOneGameOn())		
		startGameHandler(model->onlyOneGameOnID());	
	else	
		startLocation(menuScreen);	
}

void Controller::setMenuScreenHandlers()
{
	connect_once(menuScreen->startGameSignal,		bind(&Controller::startGameHandler, this, std::placeholders::_1));
	connect_once(menuScreen->startVideoSignal,		bind(&Controller::startLocation, this, screenSaver));
	controlLayer->connectEventHandler(&Controller::openSettingsHandler, this, ControlLayer::OPEN_SETTINGS);
	controlLayer->activateListeners();
}

void Controller::removeMenuScreenHandlers()
{
	menuScreen->startGameSignal.disconnect_all_slots();
	menuScreen->startVideoSignal.disconnect_all_slots();	
	controlLayer->disconnectEventHandler(ControlLayer::OPEN_SETTINGS);
	controlLayer->unActivateListeners();
}

////////////////////////////////////////////////////////////////////////////
//
//					GAME SCREEN
//
////////////////////////////////////////////////////////////////////////////	

void Controller::startGameHandler(GameId id)
{	
	if(gameSettings->isGameCurrent(id))	
		startLocation(game);	
	else	
		reloadGame(id);
}

void Controller::reloadGame(GameId id)
{
	closeCurrentLocation();

	view->showPreloader();
	gameSettings->setNextGameId(id);

	model->setDefaultGameID(id);
	model->saveUserData();

	connect_once(graphicsLoader->LoadingCompleteSignal, bind(&Controller::gameGraphicsLoadingCompleteHandler, this));
	connect_once(graphicsLoader->LoadingErrorSignal,	bind(&Controller::graphicsLoadingErrorHandler, this, std::placeholders::_1));

	//gameSettings->setTextures(id);

	graphicsLoader->setLoadingTextures(gameSettings->getGameTexturesById(id));
	graphicsLoader->load();	
}

void Controller::gameGraphicsLoadingCompleteHandler()
{
	removeGraphicsLoadingConnections();
	createGame(gameSettings->getNextGameId());
	startLocation(game);	
}

void Controller::createGame(GameId id)
{
	logger().log("CREATE GAME" + id);
	gameSettings->buildLocationData();
	gameSettings->setCurrentGame(id);
	game = gameSettings->createGame(id);	
}

void Controller::addGameHandlers()
{
	if (!model->onlyOneGameOn())
		controlLayer->showBackButton();

	game->connectEventHandler(&Controller::closeGameHandler, this, IGame::CLOSE_LOCATION);
	game->connectEventHandler(&Controller::enableGameHandler, this, IGame::ENABLE_GAME_CLOSE);
	game->connectEventHandler(&Controller::disableGameHandler, this, IGame::DISABLE_GAME_CLOSE);
	game->connectEventHandler(&Controller::showControlsHandler, this, IGame::SHOW_CONTROLS);
	game->connectEventHandler(&Controller::hideControlsHandler, this, IGame::HIDE_CONTROLS);
}

void Controller::enableGameHandler()
{
	controlLayer->activateListeners();
	controlLayer->connectEventHandler(&Controller::closeGameHandler, this, ControlLayer::CLOSE_GAME);
	controlLayer->connectEventHandler(&Controller::openSettingsHandler, this, ControlLayer::OPEN_SETTINGS);
}

void Controller::disableGameHandler()
{
	controlLayer->disconnectEventHandler(ControlLayer::CLOSE_GAME);
	controlLayer->disconnectEventHandler(ControlLayer::OPEN_SETTINGS);
	controlLayer->unActivateListeners();
}

void Controller::showControlsHandler()
{
	enableGameHandler();
}

void Controller::hideControlsHandler()
{
	disableGameHandler();
	controlLayer->hideButtons();
}

void Controller::closeGameHandler()
{	
	startLocation(menuScreen);
}

void Controller::openSettingsHandler()
{
	startLocation(controlScreen);
}

void Controller::removeGameHandlers()
{
	disableGameHandler();
	game->disconnectEventHandler(IGame::ENABLE_GAME_CLOSE);
	game->disconnectEventHandler(IGame::DISABLE_GAME_CLOSE);
	game->disconnectEventHandler(IGame::SHOW_CONTROLS);
	game->disconnectEventHandler(IGame::HIDE_CONTROLS);
	game->disconnectEventHandler(IGame::CLOSE_LOCATION);

	if (!model->onlyOneGameOn())
		controlLayer->hideBackButton();
}

////////////////////////////////////////////////////////////////////////////
//
//					SETTINGS SCREEN
//
////////////////////////////////////////////////////////////////////////////

void Controller::setConfigScreenHandlers()
{
	connect_once(controlScreen->closeLocationSignal, bind(&Controller::startMenuScreenLocation, this));		
	connect_once(controlScreen->appSettingsChangedSignal, bind(&Controller::reloadScreens, this, std::placeholders::_1, std::placeholders::_2));
}

void Controller::removeConfigScreenHandlers()
{
	controlScreen->closeLocationSignal.disconnect_all_slots();
	controlScreen->appSettingsChangedSignal.disconnect_all_slots();
}

////////////////////////////////////////////////////////////////////////////
//
//					RELOADING
//
////////////////////////////////////////////////////////////////////////////

void Controller::reloadScreens(const std::vector<Changes>& changes, bool reloadAllDesign)
{
	if (reloadAllDesign)
	{
		//removeScreenSaverHandlers();
		removeGameHandlers();
		removeMenuScreenHandlers();		
		removeConfigScreenHandlers();

		currentLocation->stop();
		view->showPreloader();
		view->clearLayers();
		controlLayer->clear();

		preloaderSettings->setTextures();
		gameSettings->setAllTextures();
		loadAllLocationsGraphics();
		return;
	}

	for(auto change : changes)
	{		
		logger().log("change.id:::::::::::::::::::  " + change.id);

		if (change.id == ChangeId::GAMES)
		{
			menuScreen->resetMenuBtnGames();			
		}
		else if (gameSettings->isGameID(change.id))
		{
			game->reset();
		}
	}

	startup();
}