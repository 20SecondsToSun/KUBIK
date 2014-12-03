#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "MenuScreen.h"
#include "SettingsScreen.h"
#include "ScreenSaver.h"
#include "ServicePopup.h"
#include "ServiceMessage.h"

using namespace std;

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
		preloader   = new Preloader(Vec2f(600.0f, 300.0f));
		servicePopup= new ServicePopup();
		view->startLocation(preloader);

		config().addCompleteListener(bind(&Controller::configLoadingCompleteHandler, this));
		config().addErrorListener(bind(&Controller::configLoadingErrorHandler, this, std::placeholders::_1));
		config().load(model);
	}

	template <typename Sig, typename F> void connect_once(Sig& sig, F&& f) 
	{
		if (!sig.num_slots()) sig.connect(std::forward<F>(f));
	}

private:

	ApplicationModel *model;
	ApplicationView	 *view;

	MenuScreen *menu;
	SettingsScreen *settings;
	GameScreen *game;
	ScreenSaver *screenSaver;
	Preloader *preloader;
	ServicePopup *servicePopup;

	void configLoadingCompleteHandler()
	{
		console()<<"config complete handler"<<endl;

		menu        = new MenuScreen(model->getGameIDsTurnOn());
		settings    = new SettingsScreen();	
		screenSaver = new ScreenSaver();	
	
		if(createGame(model->getDefaultGameID()))
		{
			graphics().setLoadingTextures(menu->getTextures());
			graphics().setLoadingTextures(settings->getTextures());
			graphics().setLoadingTextures(game->getTextures());

			graphics().addCompleteListener(bind(&Controller::allAppGraphicsLoadingCompleteHandler, this));
			graphics().addErrorListener(bind(&Controller::allAppGraphicsLoadingErrorHandler, this, std::placeholders::_1));		
			graphics().load();
		}
	}		

	void allAppGraphicsLoadingCompleteHandler()
	{
		console()<<"Graphics all Loaded::"<<endl;

		graphics().removeCompleteListener();
		graphics().removeErrorListener();

		view->init(screenSaver, menu, settings);
		game->init();

		screenSaver->addCompleteListener(bind(&Controller::completeScreenSaverHandler, this));
		screenSaver->addErrorListener(bind(&Controller::errorScreenSaverHandler, this));
		screenSaver->load();		
	}	

	void completeScreenSaverHandler()
	{
		model->setScreenSaverExist(true);
		screenSaver->removeCompleteListener();
		screenSaver->removeErrorListener();		
		startScreenSaver();	
	}

	void startScreenSaver()
	{	
		screenSaver->play();
		screenSaver->addMouseUpListener();
		connect_once(screenSaver->closeVideoSignal, bind(&Controller::closeScreenSaverHandler, this));
		view->startLocation(screenSaver);
	}

	void closeScreenSaverHandler()
	{
		screenSaver->stop();
		screenSaver->removeMouseUpListener();
		startMenuScreen();
	}

	void errorScreenSaverHandler()
	{		
		model->setScreenSaverExist(false);
		screenSaver->removeCompleteListener();
		screenSaver->removeErrorListener();
		screenSaver->removeMouseUpListener();

		startMenuScreen();
	}

	void startMenuScreen()
	{
		if(model->onlyOneGameOn())
		{
			startOneGameMode();
		}
		else
		{
			startMultiplyGameMode();
		}
	}

	void startOneGameMode()
	{		
		view->startLocation(game);
		game->addMouseUpListener();
	}

	void startMultiplyGameMode()
	{
		connect_once(menu->startGameSignal,		bind(&Controller::startGameHandler, this, std::placeholders::_1));
		connect_once(menu->startSettingsSignal, bind(&Controller::startSettingsHandler, this));
		connect_once(menu->startVideoSignal,	bind(&Controller::startVideoHandler, this));	
		connect_once(game->closeGameSignal,		bind(&Controller::closeGameHandler, this));

		menu->addMouseUpListener();	
		view->startLocation(menu);
	}

	void startSettingsHandler()
	{		
		menu->removeMouseUpListener();	
		startSettingsScreen();
	}

	void startSettingsScreen()
	{
		console()<<"startSettingsScreen::  "<<endl;
		settings->addMouseUpListener();
		connect_once(settings->closeSettingsSignal, bind(&Controller::closeSettingsHandler, this));
		view->startLocation(settings);
	}

	void closeSettingsHandler()
	{
		console()<<"closeSettingsScreen::  "<<endl;
		settings->removeMouseUpListener();
		startMenuScreen();
	}

	void startVideoHandler()
	{
		if(model->getScreenSaverExist())
		{
			console()<<"startVideo::  "<<endl;
			menu->removeMouseUpListener();	
			startScreenSaver();
		}
	}

	void allAppGraphicsLoadingErrorHandler(ServiceMessage msg)
	{	
		servicePopup->setMessage(msg);
		view->startLocation(servicePopup);
	}

	void Controller::startGameHandler(int gameId)
	{
		console()<<"game id::  "<<gameId<<endl;
		menu->removeMouseUpListener();	

		if(model->isGameCurrent(gameId))
		{
			console()<<"::load game::  "<<endl;
			view->startLocation(game);
			game->addMouseUpListener();
		}
		else
		{
			console()<<"::create new game::  "<<endl;	

			if(createGame(gameId))
			{
				console()<<"start!!! "<<endl;
				view->startLocation(preloader);	
				model->setCurrentGame(gameId);

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
	}

	bool createGame(int gameId)
	{
		if(model->checkIfGameIdPurchased(gameId))
		{
			clearPreviousGame(model->getCurrentGame());
			game = new GameScreen(gameId);
			return true;
		}
		else
			showServicePopup();

		return false;
	}	

	void showServicePopup()
	{
		console()<<" no such a game "<<endl;
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

		game->closeGameSignal.connect(bind(&Controller::closeGameHandler, this));
		game->addMouseUpListener();
		game->init();		
		view->startLocation(game);		
	}

	void gameGraphicsLoadingErrorHandler(ServiceMessage msg)
	{	
		servicePopup->setMessage(msg);
		view->startLocation(servicePopup);
	}

	void configLoadingErrorHandler(ServiceMessage msg)
	{	
		servicePopup->setMessage(msg);
		view->startLocation(servicePopup);
	}
};