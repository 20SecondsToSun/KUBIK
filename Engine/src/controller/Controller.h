#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "MenuScreen.h"
#include "TuneUpScreen.h"
#include "ScreenSaver.h"
#include "ServicePopup.h"
#include "Types.h"
#include "AppConfig.h"
#include "PhotoboothModel.h"
#include "GameConfigs.h"
#include "ScreenSaverSettings.h"
#include "TuneUpSettings.h"
#include "KubikException.h"

using namespace std;

namespace kubik
{
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
				config = new AppConfig(model);
			}
			catch(ExcConfigFileParsing exc)
			{
				servicePopupShow(exc);
			}

			loadAllLocationsConfigs();
		}

	private:

		AppConfig*	config;
		GameConfigs gamesConfigs;	

		ApplicationModel *model;

		ApplicationView	 *view;

		MenuScreen		*menu;
		TuneUpScreen    *settings;
		GameScreen		*game;
		ScreenSaver		*screenSaver;
		Preloader		*preloader;
		ServicePopup	*servicePopup;

		MenuSettings *menuSettings;
		ScreenSaverSettings *screenSaverSettings; 
		TuneUpSettings *tuneUpSettings; 

		void loadAllLocationsConfigs()
		{
			console()<<"Config Complete Handler"<<endl;

			try
			{
				gamesConfigs.load(model);
				setConfigs();
			}
			catch(ExcConfigFileParsing exc)
			{
				servicePopupShow(exc);
			}
		}

		void setConfigs()
		{
			try
			{
				menuSettings = new MenuSettings(model);	
				screenSaverSettings	= new ScreenSaverSettings(model);
				tuneUpSettings	= new TuneUpSettings(model);
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

		void setScreens()
		{
			screenSaver = new ScreenSaver();	
			menu        = new MenuScreen();
			settings    = new TuneUpScreen(tuneUpSettings, menuSettings);	
			
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

		void loadGraphics()
		{
			connect_once(graphics().completeLoadingSignal, bind(&Controller::allGraphicsLoadingCompleteHandler, this));
			connect_once(graphics().errorLoadingSignal, bind(&Controller::allGraphicsLoadingErrorHandler, this, std::placeholders::_1));

			graphics().setLoadingTextures(menuSettings->getTextures());
			graphics().setLoadingTextures(screenSaverSettings->getTextures());
			graphics().setLoadingTextures(game->getTextures());
			graphics().setLoadingTextures(settings->getTextures());		
			graphics().load();
		}

		void allGraphicsLoadingCompleteHandler()
		{
			console()<<"Graphics all Loaded:: "<<endl;
			removeGraphicsLoadingSignals();
		
			view->init(screenSaver, menu, settings);
			menu->init(menuSettings);
			settings->init();		
			game->init();		
		
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

		void removeGraphicsLoadingSignals()
		{
			graphics().completeLoadingSignal.disconnect_all_slots();	
			graphics().errorLoadingSignal.disconnect_all_slots();			
		}

		void allGraphicsLoadingErrorHandler(KubikException exc)
		{
			removeGraphicsLoadingSignals();
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
			connect_once(settings->appSettingsChangedSignal, bind(&Controller::appSettingsChangedHandler, this, std::placeholders::_1));			
			settings->addMouseUpListener();		
			view->startLocation(settings);
		}

		void closeSettingsHandler()
		{
			console()<<"Close Settings Screen :::::"<<endl;
			settings->closeLocationSignal.disconnect_all_slots();
			settings->appSettingsChangedSignal.disconnect_all_slots();
			settings->removeMouseUpListener();

			startMenuScreen();
		}

		void appSettingsChangedHandler(vector<SettingTypes> changes)
		{
			console()<<"App Settings Changed :::::"<<endl;
			settings->closeLocationSignal.disconnect_all_slots();
			settings->appSettingsChangedSignal.disconnect_all_slots();
			settings->removeMouseUpListener();

			view->startLocation(preloader);
			reloadScreens(changes);	
		}

		void reloadScreens(vector<SettingTypes> changes)
		{
			for(auto change : changes)
			{
				if(change == SettingTypes::MENU)
				{
					menu->reload();
					menuSettings->reload();
					graphics().setLoadingTextures(menuSettings->getTextures());
				}
			}

			connect_once(graphics().completeLoadingSignal, bind(&Controller::allGraphicsReloadCompleteHandler, this));
			connect_once(graphics().errorLoadingSignal, bind(&Controller::allGraphicsReloadErrorHandler, this, std::placeholders::_1));	
			graphics().load();
		}

		void allGraphicsReloadCompleteHandler()
		{
			removeGraphicsLoadingSignals();

			menu->init(menuSettings);
			startMenuScreen();
		}

		void allGraphicsReloadErrorHandler(KubikException exc)
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

			if(model->isGameCurrent(gameId))
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
					connect_once(graphics().completeLoadingSignal, bind(&Controller::gameGraphicsLoadingCompleteHandler, this));
					connect_once(graphics().errorLoadingSignal, bind(&Controller::allGraphicsLoadingErrorHandler, this, std::placeholders::_1));
					graphics().setLoadingTextures(game->getTextures());			
					graphics().load();	
				}
				catch(ExcGameDoesNotExist exc)
				{
					servicePopupShow(exc);
				}
			}	
		}		

		void createGame(int gameId)
		{
			console()<<"::create new game::"<<endl;	

			if(model->checkIfGameIdPurchased(gameId))
			{
				clearPreviousGame(model->getCurrentGame());
				model->setCurrentGame(gameId);
				game = new GameScreen(gameId, model->getGameSettingsById());	
			}
			else		
				throw ExcGameDoesNotExist();
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

		void servicePopupShow(KubikException exc)
		{
			servicePopup->setMessage(exc.what());
			view->startLocation(servicePopup);
		}
	};
}