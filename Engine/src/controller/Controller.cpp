#include "Controller.h"

using namespace kubik;
using namespace kubik::menu;
using namespace kubik::games;
using namespace kubik::games::photobooth;
using namespace kubik::games::instakub;
using namespace kubik::games::funces;
using namespace std;

Controller::Controller(ApplicationModelRef model, AppViewRef view)
{
	this->view    = view;
	this->model   = model;
	game		  = NULL;

	view->showPreloader();
	loadKubikConfig();	
}

Controller::~Controller()
{
	console()<<"~~~~~~~~~~~~~~~~~ Controller destruct ~~~~~~~~~~~~~~~~~"<<endl;
}

////////////////////////////////////////////////////////////////////////////
//
//				LOADING KUBIK CONFIG
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadKubikConfig()
{
	console()<<"start loading kubik config"<<endl;
	configLoader = ConfigLoaderRef(new ConfigLoader(model));

	connect_once(configLoader->LoadingCompleteSignal, bind(&Controller::kubikConfigLoadingCompleteHandler, this));
	connect_once(configLoader->LoadingErrorSignal,	  bind(&Controller::kubikConfigLoadingErrorHandler, this, std::placeholders::_1));

	configLoader->load();	
}

void Controller::kubikConfigLoadingCompleteHandler()
{
	removeKubikConfigLoadingConnections();
	loadSettings();
}

void Controller::kubikConfigLoadingErrorHandler(KubikException exc)
{
	removeKubikConfigLoadingConnections();
	view->showServicePopup(exc);
}

void Controller::removeKubikConfigLoadingConnections()
{
	configLoader->LoadingCompleteSignal.disconnect_all_slots();	
	configLoader->LoadingErrorSignal.disconnect_all_slots();	
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATE SETTINGS
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadSettings()
{
	console()<<"start loading other configs"<<endl;

	gameSettings		 = GameSettingsRef(new GameSettings(model));
	menuSettings		 = MenuSettingsRef(new MenuSettings(model));
	controlSettings		 = ConfigSettingsRef(new ConfigSettings(model));
	screenSaverSettings	 = ScreenSaverSettingsRef(new ScreenSaverSettings(model));

	settingsFactory().inject(controlSettings);

	list<ISettingsRef> configs;
	configs.push_back(gameSettings);
	configs.push_back(menuSettings);
	configs.push_back(controlSettings);
	configs.push_back(screenSaverSettings);

	connect_once(configLoader->LoadingCompleteSignal, bind(&Controller::configsLoadingCompleteHandler, this));
	connect_once(configLoader->LoadingErrorSignal,	  bind(&Controller::configsLoadingErrorHandler, this, std::placeholders::_1));

	configLoader->loadConfigs(configs);

	PhotoboothSettingsRef phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(GameId::PHOTOBOOTH));
	settingsFactory().inject(phbthSettings);	
}

void Controller::configsLoadingCompleteHandler()
{
	removeConfigsLoadingConnections();	
	loadAllLocationsGraphics();
}

void Controller::configsLoadingErrorHandler(KubikException exc)
{
	removeConfigsLoadingConnections();
	view->showServicePopup(exc);
}

void Controller::removeConfigsLoadingConnections()
{
	configLoader->LoadingCompleteSignal.disconnect_all_slots();	
	configLoader->LoadingErrorSignal.disconnect_all_slots();	
}

////////////////////////////////////////////////////////////////////////////
//
//				LOADING GRAPHICS
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadAllLocationsGraphics()
{	
	console()<<"start loading graphics"<<endl;
	graphicsLoader = GraphicsLoaderRef(new GraphicsLoader());

	connect_once(graphicsLoader->LoadingCompleteSignal, bind(&Controller::allGraphicsLoadingCompleteHandler, this));
	connect_once(graphicsLoader->LoadingErrorSignal, bind(&Controller::graphicsLoadingErrorHandler, this, std::placeholders::_1));

	graphicsLoader->setLoadingTextures(menuSettings->getResources());
	graphicsLoader->setLoadingTextures(screenSaverSettings->getResources());
	graphicsLoader->setLoadingTextures(controlSettings->getResources());

	graphicsLoader->setLoadingTextures(gameSettings->getGameTexturesById(GameId::PHOTOBOOTH));	
	//graphicsLoader->setLoadingTextures(gameSettings->getGameTexturesById(GameId::FUNCES));
	graphicsLoader->setLoadingTextures(gameSettings->getGameTexturesById(GameId::INSTAKUB));

	graphicsLoader->load();
}

void Controller::allGraphicsLoadingCompleteHandler()
{
	console()<<"buildData"<<endl;

	controlSettings->buildData();
	gameSettings->buildData();

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
	console()<<"create locations"<<endl;

	touchKeyboard().setup();

	screenSaver = ScreenSaverRef(new ScreenSaver(screenSaverSettings));
	menuScreen	  = MenuScreenRef(new MenuScreen(menuSettings));	
	controlLayer  = ControlLayerRef(new ControlLayer(menuSettings));	

	controlScreen = ConfigScreenRef(new ConfigScreen(controlSettings));
	controlScreen->setScreenSaverSettings(screenSaverSettings);
	controlScreen->setMenuSettings(menuSettings);
	controlScreen->setGameSettings(gameSettings);
	controlScreen->init();

	gamesFactory.reg<Photobooth>(GameId::PHOTOBOOTH, gameSettings);
	gamesFactory.reg<Instakub>(GameId::INSTAKUB, gameSettings);
	//gamesFactory.reg<Kotopoza>(GameId::INSTAKUB, gameSettings);
	createGame(model->getDefaultGameID());
	////
	view->addLayer(controlLayer);

	////startup();

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
	connect_once(currentLocation->closeLocationSignal, bind(&Controller::startLocation, this, menuScreen));
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
	{	
		startGameHandler(model->onlyOneGameOnID());
	}		
	else	
		startLocation(menuScreen);	
}

void Controller::setMenuScreenHandlers()
{
	connect_once(menuScreen->startGameSignal,		bind(&Controller::startGameHandler, this, std::placeholders::_1));
	connect_once(menuScreen->startVideoSignal,		bind(&Controller::startLocation, this, screenSaver));
	controlLayer->connectEventHandler(&Controller::openSettingsHandler, this, ControlLayer::OPEN_SETTINGS);
}

void Controller::removeMenuScreenHandlers()
{
	menuScreen->startGameSignal.disconnect_all_slots();
	menuScreen->startVideoSignal.disconnect_all_slots();	
	controlLayer->disconnectEventHandler(ControlLayer::OPEN_SETTINGS);
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
	closeCurrentLocation();//!!

	view->showPreloader();
	gameSettings->setNextGameId(id);

	model->setDefaultGameID(id);
	model->saveUserData();

	connect_once(graphicsLoader->LoadingCompleteSignal, bind(&Controller::gameGraphicsLoadingCompleteHandler, this));
	connect_once(graphicsLoader->LoadingErrorSignal,	bind(&Controller::graphicsLoadingErrorHandler, this, std::placeholders::_1));

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
	gameSettings->setCurrentGame(id);
	game = gamesFactory.create(id);		
}

void Controller::addGameHandlers()
{
	if (!model->onlyOneGameOn())
		controlLayer->showBackButton();

	game->connectEventHandler(&Controller::enableGameHandler, this, IGame::ENABLE_GAME_CLOSE);
	game->connectEventHandler(&Controller::disableGameHandler, this, IGame::DISABLE_GAME_CLOSE);
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
	
	
		//todo::  set textures
		removeConfigScreenHandlers();
		currentLocation->stop();
		view->showPreloader();
		view->clearLayers();
		controlLayer->clear();
		
		loadAllLocationsGraphics();
		return;
	}

	for(auto change : changes)
	{		
		console() << "change.id:::::::::::::::::::  " << change.id << endl;

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