#include "Controller.h"

using namespace kubik;
using namespace std;

Controller::Controller(AppModelRef model, AppViewRef view)
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
	connect_once(configLoader->LoadingErrorSignal, bind(&Controller::kubikConfigLoadingErrorHandler, this, std::placeholders::_1));

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
	graphicsLoader->setLoadingTextures(gameSettings->getGameTexturesById(GameId::INSTAKUB));	

	graphicsLoader->load();
}

void Controller::allGraphicsLoadingCompleteHandler()
{
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

	//screenSaver	  = ScreenSaverRef(new ScreenSaver(screenSaverSettings));
	//menuScreen	  = MenuScreenRef(new MenuScreen(menuSettings));	

	controlScreen = ConfigScreenRef(new ConfigScreen(controlSettings));
	controlScreen->setScreenSaverSettings(screenSaverSettings);
	controlScreen->setMenuSettings(menuSettings);
	controlScreen->setGameSettings(gameSettings);
	controlScreen->init();

	//gamesFactory.reg<Photobooth>(GameId::PHOTOBOOTH, gameSettings);
	//gamesFactory.reg<Funces>(GameId::FUNCES, gameSettings);
	//createGame(model->getDefaultGameID());	

	//startup();

	startLocation(controlScreen);
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
		startLocation(menuScreen);		
}

void Controller::startLocation(IScreenRef location)
{
	closeCurrentLocation();

	currentLocation = location;
	currentLocation->start();

	setLocationHandlers(currentLocation->getType());	
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

void Controller::setMenuScreenHandlers()
{
	if(model->onlyOneGameOn())	
		startLocation(game);	
	else	
		startMultiplyGameMode();	
}

void Controller::startMultiplyGameMode()
{
	connect_once(menuScreen->startGameSignal,		bind(&Controller::startGameHandler, this, std::placeholders::_1));
	connect_once(menuScreen->startSettingsSignal,	bind(&Controller::startLocation, this, controlScreen));
	connect_once(menuScreen->startVideoSignal,		bind(&Controller::startLocation, this, screenSaver));	
}

void Controller::removeMenuScreenHandlers()
{
	if(!model->onlyOneGameOn())	
	{
		menuScreen->startGameSignal.disconnect_all_slots();
		menuScreen->startSettingsSignal.disconnect_all_slots();
		menuScreen->startVideoSignal.disconnect_all_slots();
	}
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
	view->showPreloader();
	gameSettings->setNextGameId(id);

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
	connect_once(game->closeLocationSignal,	bind(&Controller::startLocation, this, menuScreen));	
}

void Controller::removeGameHandlers()
{
	game->closeLocationSignal.disconnect_all_slots();
}

////////////////////////////////////////////////////////////////////////////
//
//					SETTINGS SCREEN
//
////////////////////////////////////////////////////////////////////////////

void Controller::setConfigScreenHandlers()
{
	connect_once(controlScreen->closeLocationSignal, bind(&Controller::startLocation, this, menuScreen));		
	connect_once(controlScreen->appSettingsChangedSignal, bind(&Controller::appSettingsChangedHandler, this, std::placeholders::_1));	
}

void Controller::appSettingsChangedHandler(vector<Changes> changes)
{
	if(changes.empty())
		startLocation(menuScreen);
	else
		reloadScreens(changes);	
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

void Controller::reloadScreens(vector<Changes> changes)
{
	bool toReload = false;

	for(auto change : changes)
	{		
		ISettingsRef settings = getSettingsByChangeId(change.id);

		if(change.texReload)
		{
			view->showPreloader();
			settings->setTextures();
			graphicsLoader->setLoadingTextures(settings->getResources());			
			toReload = true;
		}
		else if (change.id == ChangeId::GAMES)		
			menuScreen->resetMenuBtnGames();		
		else			
			getScreenByChangeId(change.id)->reset();
	}
	console()<<"NEED TO RELOAD:::::::::::::::::::  "<<toReload<<endl;

	if(toReload)
	{
		connect_once(graphicsLoader->LoadingCompleteSignal, bind(&Controller::allGraphicsReloadCompleteHandler, this, changes));
		connect_once(graphicsLoader->LoadingErrorSignal, bind(&Controller::graphicsLoadingErrorHandler, this, placeholders::_1));	
		graphicsLoader->load();
	}
	else
		startAfterReload();
}

void Controller::allGraphicsReloadCompleteHandler(vector<Changes> changes)
{
	removeGraphicsLoadingConnections();

	GameId id = GameId::UNDEFINED;

	for(auto change : changes)	
		if (change.id == ChangeId::GAMES)
			id = getReloadGameId();
		else
			getScreenByChangeId(change.id)->reset();

	startAfterReload(id);
}

GameId Controller::getReloadGameId()
{
	GameId reloadGameId = GameId::UNDEFINED;
	GameSettings::GamesDataStruct gameData = gameSettings->getData();

	if(!gameSettings->isCurrentGameInSwitchOnGames() && gameData.getCountSwitchOnGames() == 1)
	{	
		vector<GamesInfo> games = gameData.getGames();

		for (size_t i = 0; i < games.size(); i++)		
			if( games[i].isOn)
			{
				reloadGameId = games[i].id;
				break;
			}				
	}
	return  reloadGameId;
}

void Controller::startAfterReload(GameId id)
{
	if(id != GameId::UNDEFINED)	
		startGameHandler(id);	
	else
		startLocation(menuScreen);
}

ISettingsRef Controller::getSettingsByChangeId(ChangeId id)
{
	ISettingsRef settings;

	if(gameSettings->isGameID(id))			
		settings = gameSettings->get((game::id)id);	
	else if (id == ChangeId::MENU)
		settings = menuSettings;	
	else if (id == ChangeId::SCREENSAVER)	
		settings = screenSaverSettings;

	return settings;
}

IScreenRef Controller::getScreenByChangeId(ChangeId id)
{
	IScreenRef screen;
	if(gameSettings->isGameID(id))
		screen   = game;
	else if (id == ChangeId::MENU)
		screen = menuScreen;
	else if (id == ChangeId::SCREENSAVER)
		screen = screenSaver;

	return screen;
}