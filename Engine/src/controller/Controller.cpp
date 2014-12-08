#include "Controller.h"

using namespace kubik;
using namespace std;

Controller::Controller(ApplicationView* view)
{
	this->view  = view;
	game		= NULL;
}

void Controller::initLoad()
{
	preloader		= new Preloader();
	servicePopup	= new ServicePopup();

	view->startLocation(preloader);	

	try
	{
		model = new ApplicationModel();
		model->load();
	}
	catch(ExcConfigFileParsing exc)
	{
		servicePopupShow(exc);
	}

	loadAllLocationsConfigs();
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
		gameSettings		 = new GameSettings(model);
		menuSettings		 = new MenuSettings(model);	
		screenSaverSettings	 = new ScreenSaverSettings(model);
		tuneUpSettings		 = new TuneUpSettings(model);

		gameSettings->loadGamesSettings();

		setScreens();
	}
	catch(ExcConfigFileParsing exc)
	{
		servicePopupShow(exc);
	}
	catch(ExcBigFileSizeOfScreenSaver exc)
	{
		servicePopupShow(exc);
	}
	catch(...)
	{

	}
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATING LOCATIONS
//
////////////////////////////////////////////////////////////////////////////

void Controller::setScreens()
{
	screenSaver = new ScreenSaver();	
	menu        = new MenuScreen();
	settings    = new TuneUpScreen(tuneUpSettings, menuSettings, gameSettings);	

	try
	{
		createGame(model->getDefaultGameID());
		loadGraphics();
	}
	catch(ExcGameDoesNotExist exc)
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
	graphicsLoader = new Graphics();

	connect_once(graphicsLoader->completeLoadingSignal, bind(&Controller::allGraphicsLoadingCompleteHandler, this));
	connect_once(graphicsLoader->errorLoadingSignal, bind(&Controller::allGraphicsLoadingErrorHandler, this, std::placeholders::_1));

	graphicsLoader->setLoadingTextures(menuSettings->getTextures());
	graphicsLoader->setLoadingTextures(screenSaverSettings->getTextures());
	graphicsLoader->setLoadingTextures(game->getTextures());
	graphicsLoader->setLoadingTextures(settings->getTextures());

	graphicsLoader->load();
}

void Controller::allGraphicsLoadingCompleteHandler()
{
	console()<<"Graphics all Loaded:: "<<endl;

	removeGraphicsLoadingSignals();

	// INIT LOCATIONS

	view->init(screenSaver, menu, settings);
	menu->init(menuSettings);		
	game->init();
	settings->init();	

	if(screenSaverSettings->isExist())
	{
		screenSaver->init(screenSaverSettings);
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

void Controller::allGraphicsLoadingErrorHandler(KubikException exc)
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
	console()<<"startScreenSaver::  "<<endl;		

	screenSaver->start();
	screenSaver->addMouseUpListener();
	connect_once(screenSaver->closeLocationSignal, bind(&Controller::closeScreenSaverHandler, this));
	view->startLocation(screenSaver);
}

void Controller::closeScreenSaverHandler()
{
	screenSaver->closeLocationSignal.disconnect_all_slots();
	screenSaver->stop();
	screenSaver->removeMouseUpListener();
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
		resetGame();
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
	console()<<"Start Settings Screen :::::"<<endl;
	connect_once(settings->closeLocationSignal, bind(&Controller::closeSettingsHandler, this));			
	connect_once(settings->appSettingsChangedSignal, bind(&Controller::appSettingsChangedHandler, this, std::placeholders::_1));			
	settings->addMouseUpListener();		
	view->startLocation(settings);
}

void Controller::closeSettingsHandler()
{
	console()<<"Close Settings Screen :::::"<<endl;
	settings->closeLocationSignal.disconnect_all_slots();
	settings->appSettingsChangedSignal.disconnect_all_slots();
	settings->removeMouseUpListener();

	startMenuScreen();
}

void Controller::appSettingsChangedHandler(vector<SettingTypes> changes)
{
	console()<<"App Settings Changed :::::"<<endl;
	settings->closeLocationSignal.disconnect_all_slots();
	settings->appSettingsChangedSignal.disconnect_all_slots();
	settings->removeMouseUpListener();

	view->startLocation(preloader);
	reloadScreens(changes);	
}

void Controller::reloadScreens(vector<SettingTypes> changes)
{
	for(auto change : changes)
	{
		if(change == SettingTypes::MENU)
		{
			menu->reload();
			menuSettings->reload();
			graphicsLoader->setLoadingTextures(menuSettings->getTextures());
		}
	}

	connect_once(graphicsLoader->completeLoadingSignal, bind(&Controller::allGraphicsReloadCompleteHandler, this));
	connect_once(graphicsLoader->errorLoadingSignal, bind(&Controller::allGraphicsReloadErrorHandler, this, std::placeholders::_1));	
	
	graphicsLoader->load();
}

void Controller::allGraphicsReloadCompleteHandler()
{
	removeGraphicsLoadingSignals();

	menu->init(menuSettings);
	startMenuScreen();
}

void Controller::allGraphicsReloadErrorHandler(KubikException exc)
{
	removeGraphicsLoadingSignals();
	servicePopupShow(exc);
}

////////////////////////////////////////////////////////////////////////////
//
//					GAME SCREEN
//
////////////////////////////////////////////////////////////////////////////	

void Controller::startGameHandler(int gameId)
{
	console()<<"game id:: "<<gameId<<endl;
	menu->removeMouseUpListener();

	if(gameSettings->isGameCurrent(gameId))
	{
		resetGame();
		startGame();
	}
	else
	{
		try
		{
			createGame(gameId);

			view->startLocation(preloader);	
			connect_once(graphicsLoader->completeLoadingSignal, bind(&Controller::gameGraphicsLoadingCompleteHandler, this));
			connect_once(graphicsLoader->errorLoadingSignal, bind(&Controller::allGraphicsLoadingErrorHandler, this, std::placeholders::_1));
			graphicsLoader->setLoadingTextures(game->getTextures());

			graphicsLoader->load();	
		}
		catch(ExcGameDoesNotExist exc)
		{
			servicePopupShow(exc);
		}
	}	
}		

void Controller::createGame(int gameId)
{
	console()<<"::create new game::"<<endl;	

	if(model->checkIfGameIdPurchased(gameId))
	{
		clearPreviousGame(gameSettings->getCurrentGame());
		gameSettings->setCurrentGame(gameId);
		game = new GameScreen(gameId, gameSettings);	
	}
	else		
		throw ExcGameDoesNotExist();
}

void Controller::closeGameHandler()
{
	console()<<"::::closeGameHandler fired::::"<<endl;
	game->removeMouseUpListener();
	menu->addMouseUpListener();	
	view->startLocation(menu);	
}

void Controller::clearPreviousGame(int id)
{
	if(game)
	{
		delete game;
		game = NULL;
	}
}

void Controller::gameGraphicsLoadingCompleteHandler()
{
	graphicsLoader->completeLoadingSignal.disconnect_all_slots();	
	graphicsLoader->errorLoadingSignal.disconnect_all_slots();

	game->init();
	startGame();	
}	

void Controller::resetGame()
{
	game->reset();
}

void Controller::startGame()
{	
	console()<<"START GAME::"<<endl;
	game->addMouseUpListener();
	view->startLocation(game);
	connect_once(game->closeLocationSignal,	bind(&Controller::closeGameHandler, this));
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