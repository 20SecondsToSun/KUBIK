#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "MenuScreen.h"
#include "SettingsScreen.h"
#include "ScreenSaver.h"
#include "ServicePopup.h"
#include "ServiceMessage.h"
#include "Types.h"
#include "AppConfig.h"
#include "PhotoboothModel.h"
#include "GameConfigs.h"

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

		config.addCompleteListener(bind(&Controller::configLoadingCompleteHandler, this));
		config.addErrorListener(bind(&Controller::configLoadingErrorHandler, this, std::placeholders::_1));
		config.load(model);
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

	void configLoadingCompleteHandler()
	{
		console()<<"Config Complete Handler"<<endl;

		gamesConfigs.addCompleteListener(bind(&Controller::allConfigsLoadingCompleteHandler, this));
		gamesConfigs.addErrorListener(bind(&Controller::allConfigsLoadingErrorHandler, this, std::placeholders::_1));
		gamesConfigs.load(model);	
	}

	void allConfigsLoadingCompleteHandler()
	{
		menu        = new MenuScreen();
		settings    = new SettingsScreen();	
		screenSaver = new ScreenSaver();	
	
		if(createGame(model->getDefaultGameID()))
		{
			graphics().setLoadingTextures(menu->getTextures());
			graphics().setLoadingTextures(settings->getTextures());
			graphics().setLoadingTextures(game->getTextures());
			graphics().setLoadingTextures(screenSaver->getTextures());

			graphics().addCompleteListener(bind(&Controller::allAppGraphicsLoadingCompleteHandler, this));
			graphics().addErrorListener(bind(&Controller::allAppGraphicsLoadingErrorHandler, this, std::placeholders::_1));		
			graphics().load();		
		}
	}

	void allConfigsLoadingErrorHandler(ServiceMessage msg)
	{	
		servecePopupShow(msg);
	}

	void allAppGraphicsLoadingCompleteHandler()
	{
		console()<<"Graphics all Loaded::"<<endl;

		graphics().removeCompleteListener();
		graphics().removeErrorListener();

		view->init(screenSaver, menu, settings);
		menu->init(model->getGameIDsTurnOn());
		settings->init();		
		game->init();

		bool screenSaverExist  = !screenSaver->isEmpty();
		model->setScreenSaverExist(screenSaverExist);

		if(screenSaverExist)
		{
			screenSaver->init();
			startScreenSaver();	
		}
		else if(screenSaver->isError())
		{			
			servecePopupShow(screenSaver->getMessage());
		}
		else
		{
			startMenuScreen();
		}	
	}		

	////////////////////////////////////////////////////////////////////////////
	//
	//				SCREEN SAVER
	//
	////////////////////////////////////////////////////////////////////////////

	void startScreenSaver()
	{	
		console()<<"startScreenSaver::  "<<endl;		

		screenSaver->play();
		screenSaver->addMouseUpListener();
		connect_once(screenSaver->closeLocationSignal, bind(&Controller::closeScreenSaverHandler, this));
		view->startLocation(screenSaver);
	}

	void closeScreenSaverHandler()
	{
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

			graphics().setLoadingTextures(game->getTextures());
			graphics().addCompleteListener(bind(&Controller::gameGraphicsLoadingCompleteHandler, this));
			graphics().addErrorListener(bind(&Controller::gameGraphicsLoadingErrorHandler, this, std::placeholders::_1));
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
			ServiceMessage msg(102);
			noSuchGameExist(msg);
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
		graphics().removeCompleteListener();
		graphics().removeErrorListener();
		
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

	void noSuchGameExist(ServiceMessage msg)
	{
		servecePopupShow(msg);
	}	

	void allAppGraphicsLoadingErrorHandler(ServiceMessage msg)
	{	
		servecePopupShow(msg);
	}

	void gameGraphicsLoadingErrorHandler(ServiceMessage msg)
	{	
		servecePopupShow(msg);
	}

	void configLoadingErrorHandler(ServiceMessage msg)
	{	
		servecePopupShow(msg);
	}

	void servecePopupShow(ServiceMessage msg)
	{
		servicePopup->setMessage(msg);
		view->startLocation(servicePopup);
	}

	boost::signals2::connection closeSettingsCon;//, startGameCon, startSettingsCon, startVideoCon;
};