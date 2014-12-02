#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "MenuScreen.h"
#include "SettingsScreen.h"

using namespace std;

class Controller
{
public:

	Controller(ApplicationModel *model, ApplicationView* view)
	{
		this->model = model;
		this->view  = view;
	}

	void initLoad()
	{
		config().addCompleteListener(bind(&Controller::configLoadingCompleteHandler, this));
		config().addErrorListener(bind(&Controller::configLoadingErrorHandler, this));
		config().load(model);
	}

private:

	ApplicationModel *model;
	ApplicationView	 *view;

	MenuScreen *menu;
	SettingsScreen *settings;
	GameScreen *game;

	void configLoadingCompleteHandler()
	{
		console()<<"config complete handler"<<endl;

		menu = new MenuScreen(model->getGameIDsTurnOn());
		settings = new SettingsScreen();		
		game = new GameScreen(model->getDefaultGameID());

		graphics().setLoadingTextures(menu->getTextures());
		graphics().setLoadingTextures(settings->getTextures());
		graphics().setLoadingTextures(game->getTextures());

		graphics().addCompleteListener(bind(&Controller::allAppGraphicsLoadingCompleteHandler, this));
		graphics().addErrorListener(bind(&Controller::allAppGraphicsLoadingErrorHandler, this));		
		graphics().load();
	}		

	void allAppGraphicsLoadingCompleteHandler()
	{
		console()<<"Graphics all Loaded::"<<endl;

		graphics().removeCompleteListener();
		graphics().removeErrorListener();
	
		menu->startGameSignal.connect(bind(&Controller::startGameHandler, this, std::placeholders::_1));
		menu->addMouseUpListener();	

		view->init(menu, settings);
		view->startMenu();
	
		game->closeGameSignal.connect(bind(&Controller::closeGameHandler, this));
		game->init();
	}

	void allAppGraphicsLoadingErrorHandler()
	{

	}

	void Controller::startGameHandler(int gameId)
	{
		console()<<"game id::  "<<gameId<<endl;
		menu->removeMouseUpListener();	

		if(model->isGameCurrent(gameId))
		{
			console()<<"::load game::  "<<endl;
			view->startGame(game);
			game->addMouseUpListener();
		}
		else
		{
			view->setPreloading(true);
			console()<<"::create new game::  "<<endl;
			clearPreviousGame(model->getCurrentGame());
			model->setCurrentGame(gameId);

			game = new GameScreen(gameId);		

			graphics().setLoadingTextures(game->getTextures());
			graphics().addCompleteListener(bind(&Controller::gameGraphicsLoadingCompleteHandler, this));
			graphics().addErrorListener(bind(&Controller::gameGraphicsLoadingErrorHandler, this));
			graphics().load();
		}
	}

	void closeGameHandler()
	{
		console()<<"::::closeGameHandler fired::::"<<endl;
		game->removeMouseUpListener();
		menu->addMouseUpListener();	
		view->startMenu();	
	}

	void clearPreviousGame(int id)
	{
		delete game;
	}

	void gameGraphicsLoadingCompleteHandler()
	{
		graphics().removeCompleteListener();
		graphics().removeErrorListener();

		game->closeGameSignal.connect(bind(&Controller::closeGameHandler, this));
		game->addMouseUpListener();
		game->init();		
		view->startGame(game);
		view->setPreloading(false);
	}

	void gameGraphicsLoadingErrorHandler()
	{

	}

	void configLoadingErrorHandler()
	{

	}
};