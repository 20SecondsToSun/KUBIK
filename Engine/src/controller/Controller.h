#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "MenuScreen.h"
#include "SettingsScreen.h"
#include "ScreenSaver.h"
#include "ServicePopup.h"
#include "Types.h"
#include "AppConfig.h"
#include "PhotoboothModel.h"
#include "GameConfigs.h"
#include "ScreenSaverSettings.h"
#include "KubikException.h"

using namespace std;
using namespace kubik;

class Controller
{
public:

	Controller(ApplicationModel *model, ApplicationView* view)
	{
		this->model = model;
		this->view  = view;
		game		= NULL;
	}

	void initLoad()
	{
		preloader   = new Preloader();
		servicePopup= new ServicePopup();
		view->startLocation(preloader);	

		try
		{
			config.load(model);
		}
		catch(ExcConfigFileParsing exc)
		{
			servicePopupShow(exc);
		}

		loadAllLocationsConfigs();
	}

private:

	AppConfig	config;
	GameConfigs gamesConfigs;	

	ApplicationModel *model;

	ApplicationView	 *view;

	MenuScreen		*menu;
	SettingsScreen  *settings;
	GameScreen		*game;
	ScreenSaver		*screenSaver;
	Preloader		*preloader;
	ServicePopup	*servicePopup;

	MenuSettings *menuSettings;
	ScreenSaverSettings *screenSaverSettings; 

	void loadAllLocationsConfigs()
	{
		console()<<"Config Complete Handler"<<endl;

		try
		{
			gamesConfigs.load(model);
		}
		catch(ExcConfigFileParsing exc)
		{
			servicePopupShow(exc);
		}	

		setConfigs();
	}

	void setConfigs()
	{
		menuSettings		    = new MenuSettings(model);		

		try
		{
			screenSaverSettings	= new ScreenSaverSettings(model);
		}
		catch(ExcBigFileSizeOfScreenSaver exc)
		{
			servicePopupShow(exc);
		}

		screenSaver = new ScreenSaver(screenSaverSettings);	
		menu        = new MenuScreen(menuSettings);
		settings    = new SettingsScreen();	


		if(createGame(model->getDefaultGameID()))
		{
			connect_once(graphics().completeLoadingSignal, bind(&Controller::allGraphicsLoadingCompleteHandler, this));
			connect_once(graphics().errorLoadingSignal, bind(&Controller::allGraphicsLoadingErrorHandler, this, std::placeholders::_1));

			graphics().setLoadingTextures(menu->getTextures());
			graphics().setLoadingTextures(settings->getTextures());
			graphics().setLoadingTextures(game->getTextures());
			graphics().setLoadingTextures(screenSaver->getTextures());		
			graphics().load();	
		}
	}

	void allGraphicsLoadingCompleteHandler()
	{
		console()<<"Graphics all Loaded:: "<<screenSaverSettings->isExist()<<endl;

		graphics().completeLoadingSignal.disconnect_all_slots();	
		graphics().errorLoadingSignal.disconnect_all_slots();	

		view->init(screenSaver, menu, settings);
		menu->init(model->getGameIDsTurnOn());

		settings->init();		
		game->init();		

		if(screenSaverSettings->isExist())
		{
			screenSaver->init();
			startScreenSaver();	
		}
		else
		{
			startMenuScreen();
		}	
	}

	void allGraphicsLoadingErrorHandler(KubikException exc)
	{
		graphics().completeLoadingSignal.disconnect_all_slots();	
		graphics().errorLoadingSignal.disconnect_all_slots();	
		servicePopupShow(exc);
	}

	////////////////////////////////////////////////////////////////////////////
	//
	//				SCREEN SAVER
	//
	////////////////////////////////////////////////////////////////////////////

	void startScreenSaver()
	{	
		console()<<"startScreenSaver::  "<<endl;		

		screenSaver->start();
		screenSaver->addMouseUpListener();
		connect_once(screenSaver->closeLocationSignal, bind(&Controller::closeScreenSaverHandler, this));
		view->startLocation(screenSaver);
	}

	void closeScreenSaverHandler()
	{
		screenSaver->closeLocationSignal.disconnect_all_slots();
		screenSaver->stop();
		screenSaver->removeMouseUpListener();
		startMenuScreen();
	}

	void startScreenSaverHandler()
	{
		if(model->getScreenSaverExist())
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

	void startMenuScreen()
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

	void startMultiplyGameMode()
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

	void startSettingsHandler()
	{		
		menu->removeMouseUpListener();	
		startSettingsScreen();
	}

	void startSettingsScreen()
	{
		console()<<"Start Settings Screen :::::"<<endl;
		connect_once(settings->closeLocationSignal, bind(&Controller::closeSettingsHandler, this));			
		settings->addMouseUpListener();		
		view->startLocation(settings);
	}

	void closeSettingsHandler()
	{
		console()<<"Close Settings Screen :::::"<<endl;
		settings->closeLocationSignal.disconnect_all_slots();
		settings->removeMouseUpListener();
		startMenuScreen();
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

		if(model->isGameCurrent(gameId))
		{
			console()<<"::load game::"<<endl;
			resetGame();
			startGame();
		}
		else if(createGame(gameId))
		{
			console()<<"start!!! "<<endl;
			view->startLocation(preloader);		

			connect_once(graphics().completeLoadingSignal, bind(&Controller::gameGraphicsLoadingCompleteHandler, this));
			connect_once(graphics().errorLoadingSignal, bind(&Controller::allGraphicsLoadingErrorHandler, this, std::placeholders::_1));
			graphics().setLoadingTextures(game->getTextures());			
			graphics().load();				
		}
		else
		{
			menu->addMouseUpListener();	
		}		
	}		

	bool createGame(int gameId)
	{
		console()<<"::create new game::"<<endl;	

		if(model->checkIfGameIdPurchased(gameId))
		{
			clearPreviousGame(model->getCurrentGame());
			model->setCurrentGame(gameId);
			game = new GameScreen(gameId, model->getGameSettingsById());
			return true;
		}
		else
		{
			servicePopupShow(ExcGameDoesNotExist());
		}

		return false;
	}

	void closeGameHandler()
	{
		console()<<"::::closeGameHandler fired::::"<<endl;
		game->removeMouseUpListener();
		menu->addMouseUpListener();	
		view->startLocation(menu);	
	}

	void clearPreviousGame(int id)
	{
		if(game)
		{
			delete game;
			game = NULL;
		}
	}

	void gameGraphicsLoadingCompleteHandler()
	{
		graphics().completeLoadingSignal.disconnect_all_slots();	
		graphics().errorLoadingSignal.disconnect_all_slots();
		
		game->init();
		startGame();	
	}	

	void resetGame()
	{
		game->reset();
	}

	void startGame()
	{		
		game->addMouseUpListener();
		view->startLocation(game);
		connect_once(game->closeLocationSignal,	bind(&Controller::closeGameHandler, this));
	}

	////////////////////////////////////////////////////////////////////////////
	//
	//					SERVICE POPUP SCREEN
	//
	////////////////////////////////////////////////////////////////////////////

	void servicePopupShow(KubikException exc)
	{
		servicePopup->setMessage(exc.what());
		view->startLocation(servicePopup);
	}
};