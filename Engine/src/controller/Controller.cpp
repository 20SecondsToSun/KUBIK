#include "Controller.h"
#include "cinder/params/Params.h"

using namespace kubik;
using namespace std;

Controller::Controller(shared_ptr<ApplicationView> view)
{
	this->view  = view;
	game		= NULL;
}

Controller::~Controller()
{
	console()<<"~~~~~~~~~~~~~~~~~ Controller destruct()~~~~~~~~~~~~~~~~~"<<endl;
}

void Controller::initLoad()
{	
	preloader		= shared_ptr<Preloader>(new Preloader());	
	servicePopup	= shared_ptr<ServicePopup>(new ServicePopup());// font memory leak
	view->startLocation(preloader);	

	try
	{
		model = shared_ptr<ApplicationModel>(new ApplicationModel());
		model->load();

		loadAllLocationsConfigs();
	}
	catch(ExcConfigFileParsing exc)
	{
		servicePopupShow(exc);
	}	
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATING CONFIGS
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadAllLocationsConfigs()
{	
	try
	{
		gameSettings		 = shared_ptr<GameSettings>(new GameSettings(model));
		menuSettings		 = shared_ptr<MenuSettings>(new MenuSettings(model));
		tuneUpSettings		 = shared_ptr<TuneUpSettings>(new TuneUpSettings(model));
		screenSaverSettings	 = shared_ptr<ScreenSaverSettings>(new ScreenSaverSettings(model));
		loadGraphics();
	}
	catch(ExcConfigFileParsing exc)
	{
		servicePopupShow(exc);
	}
	catch(ExcBigFileSizeOfScreenSaver exc)
	{
		servicePopupShow(exc);
	}
}

////////////////////////////////////////////////////////////////////////////
//
//				LOADING GRAPHICS
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadGraphics()
{	
	console()<<"start loading graphics"<<endl;
	graphicsLoader = shared_ptr<Graphics>(new Graphics());

	connect_once(graphicsLoader->completeLoadingSignal, bind(&Controller::allGraphicsLoadingCompleteHandler, this));
	connect_once(graphicsLoader->errorLoadingSignal, bind(&Controller::graphicsLoadErrorHandler, this, std::placeholders::_1));
	graphicsLoader->setLoadingTextures(menuSettings->getResources());
	graphicsLoader->setLoadingTextures(screenSaverSettings->getResources());
	graphicsLoader->setLoadingTextures(tuneUpSettings->getResources());
	graphicsLoader->setLoadingTextures(gameSettings->getActiveGameTextures());	
	graphicsLoader->load();
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATING LOCATIONS
//
////////////////////////////////////////////////////////////////////////////

void Controller::allGraphicsLoadingCompleteHandler()
{
	console()<<"Graphics all Loaded:: "<<endl;
	removeGraphicsLoadingSignals();

	screenSaver = shared_ptr<ScreenSaver>(new ScreenSaver(screenSaverSettings));		
	menu        = shared_ptr<MenuScreen>(new MenuScreen(menuSettings));
	
	settings    = shared_ptr<TuneUpScreen>(new TuneUpScreen(tuneUpSettings, screenSaverSettings, menuSettings, gameSettings));
	
	gamesFactory.reg<Photobooth>(gameId::PHOTOBOOTH, gameSettings->get(gameId::PHOTOBOOTH));
	gamesFactory.reg<Funces>    (gameId::FUNCES,	 gameSettings->get(gameId::FUNCES));	
	
	createGame(model->getDefaultGameID());	
	firstStart();	
}

void Controller::firstStart()
{
	if(screenSaverSettings->isExist())
	{
		startScreenSaver();	
	}
	else
	{
		startMenuScreen();
	}	
}

void Controller::removeGraphicsLoadingSignals()
{
	graphicsLoader->completeLoadingSignal.disconnect_all_slots();	
	graphicsLoader->errorLoadingSignal.disconnect_all_slots();			
}

void Controller::graphicsLoadErrorHandler(KubikException exc)
{
	removeGraphicsLoadingSignals();
	servicePopupShow(exc);
}

////////////////////////////////////////////////////////////////////////////
//
//				SCREEN SAVER
//
////////////////////////////////////////////////////////////////////////////

void Controller::startScreenSaver()
{	
	screenSaver->start();
	screenSaver->addMouseUpListener();
	connect_once(screenSaver->closeLocationSignal, bind(&Controller::closeScreenSaverHandler, this));
	view->startLocation(screenSaver);
}

void Controller::closeScreenSaverHandler()
{
	screenSaver->closeLocationSignal.disconnect_all_slots();
	screenSaver->removeMouseUpListener();
	screenSaver->stop();	
	startMenuScreen();
}

void Controller::startScreenSaverHandler()
{
	if(screenSaverSettings->isExist())
	{
		menu->removeMouseUpListener();	
		startScreenSaver();
	}
}

////////////////////////////////////////////////////////////////////////////
//
//					MENU SCREEN
//
////////////////////////////////////////////////////////////////////////////

void Controller::startMenuScreen()
{
	if(model->onlyOneGameOn())
	{
		startGame();
	}
	else
	{
		startMultiplyGameMode();
	}
}

void Controller::startMultiplyGameMode()
{
	connect_once(menu->startGameSignal,		bind(&Controller::startGameHandler, this, std::placeholders::_1));
	connect_once(menu->startSettingsSignal, bind(&Controller::startSettingsHandler, this));
	connect_once(menu->startVideoSignal,	bind(&Controller::startScreenSaverHandler, this));		

	menu->addMouseUpListener();	
	view->startLocation(menu);
}

////////////////////////////////////////////////////////////////////////////
//
//					GAME SCREEN
//
////////////////////////////////////////////////////////////////////////////	

void Controller::startGameHandler(int gameId)
{
	menu->removeMouseUpListener();

	if(gameSettings->isGameCurrent(gameId))
	{
		startGame();
	}
	else
	{
		view->startLocation(preloader);	
		gameSettings->setNextGameId(gameId);
			
		connect_once(graphicsLoader->completeLoadingSignal, bind(&Controller::gameGraphicsLoadingCompleteHandler, this));
		connect_once(graphicsLoader->errorLoadingSignal,	bind(&Controller::graphicsLoadErrorHandler, this, std::placeholders::_1));
		graphicsLoader->setLoadingTextures(gameSettings->getGameTexturesById(gameId));	
		graphicsLoader->load();	
	}	
}	

void Controller::gameGraphicsLoadingCompleteHandler()
{
	removeGraphicsLoadingSignals();

	createGame(gameSettings->getNextGameId());
	startGame();	
}

void Controller::createGame(int gameId)
{
	console()<<"::create new game::"<<gameId<<endl;	
	gameSettings->setCurrentGame(gameId);
	game = gamesFactory.create(gameId);		
}

void Controller::startGame()
{	
	console()<<"START GAME::"<<endl;
	game->addMouseUpListener();
	view->startGameLocation(game);
	connect_once(game->closeLocationSignal,	bind(&Controller::closeGameHandler, this));	
}

void Controller::closeGameHandler()
{
	game->removeMouseUpListener();
	menu->addMouseUpListener();	
	view->startLocation(menu);	
}

////////////////////////////////////////////////////////////////////////////
//
//					SETTINGS SCREEN
//
////////////////////////////////////////////////////////////////////////////

void Controller::startSettingsHandler()
{		
	menu->removeMouseUpListener();	
	startSettingsScreen();
}

void Controller::startSettingsScreen()
{
	connect_once(settings->closeLocationSignal, bind(&Controller::closeSettingsHandler, this));			
	connect_once(settings->appSettingsChangedSignal, bind(&Controller::appSettingsChangedHandler, this, std::placeholders::_1));	

	settings->addMouseUpListener();	
	settings->startUpParams();

	view->startLocation(settings);
}

void Controller::closeSettingsHandler()
{
	settingsScreenRemoveListeners();
	startMenuScreen();
}

void Controller::settingsScreenRemoveListeners()
{
	settings->closeLocationSignal.disconnect_all_slots();
	settings->appSettingsChangedSignal.disconnect_all_slots();
	settings->removeMouseUpListener();
}

void Controller::appSettingsChangedHandler(vector<Changes> changes)
{
	settingsScreenRemoveListeners();
	reloadScreens(changes);	
}

void Controller::reloadScreens(vector<Changes> changes)
{
	this->reloadSettingsChanges = changes;
	bool toReload = false;

	for(auto change : changes)
	{
		int id = change.id;
		bool texReload = change.texReload;

		bool isGame = gameSettings->isGameID(id);
		LocMapper mapper   = getLocationPair(id);
		
		if(!isGame || (isGame && gameSettings->isGameCurrent(id)))
		{
			if(texReload)
			{
				mapper.settings->setTextures();
				graphicsLoader->setLoadingTextures(mapper.settings->getResources());
				view->startLocation(preloader);
				toReload = true;
			}
			else
			{
				mapper.screen->reset(mapper.settings);
			}
		}		
	}

	console()<<"NEED TO RELOAD?::  "<<toReload<<endl;

	if(toReload)
	{
		connect_once(graphicsLoader->completeLoadingSignal, bind(&Controller::allGraphicsReloadCompleteHandler, this));
		connect_once(graphicsLoader->errorLoadingSignal, bind(&Controller::graphicsLoadErrorHandler, this, std::placeholders::_1));	
		graphicsLoader->load();
	}
	else
	{
		startMenuScreen();
	}
}

void Controller::allGraphicsReloadCompleteHandler()
{
	removeGraphicsLoadingSignals();

	for(auto change : reloadSettingsChanges)
	{
		LocMapper mapper = getLocationPair(change.id);
		mapper.screen->reset(mapper.settings);
	}

	startMenuScreen();
}

Controller::LocMapper Controller::getLocationPair(int id)
{
	LocMapper mapper;
	console()<<"game!!!!!!!!!!!!!!!!!"<<id<<gameSettings->isGameID(id)<<endl;
	if(gameSettings->isGameID(id))
	{
		
		mapper.screen   = game;
		mapper.settings = gameSettings->get(id);
	}
	else if (id == LocationID::MENU)
	{
		mapper.screen = menu;
		mapper.settings = menuSettings;
	}

	return mapper;
}

////////////////////////////////////////////////////////////////////////////
//
//					SERVICE POPUP SCREEN
//
////////////////////////////////////////////////////////////////////////////

void Controller::servicePopupShow(KubikException exc)
{
	servicePopup->setMessage(exc.what());
	view->startLocation(servicePopup);
}