#include "ConfigScreen.h"

using namespace std;
using namespace params;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace kubik::config;
using namespace kubik;

ConfigScreen::ConfigScreen(ISettingsRef config):IScreen(ScreenId::CONFIG)
{
	configSettings	= static_pointer_cast<ConfigSettings>(config);
}

ConfigScreen::~ConfigScreen()
{	
	console()<<"~~~~~~~~~~~~~~~ TuneUpScreen destructor ~~~~~~~~~~~~~~~"<<endl;
	closeBtnListener.disconnect();	
	appSettingsChgListener.disconnect();	
}

void ConfigScreen::setScreenSaverSettings(ScreenSaverSettingsRef screenSaverSettings)
{
	this->screenSaverSettings = screenSaverSettings;
}

void ConfigScreen::setMenuSettings(MenuSettingsRef menuSettings)
{
	this->menuSettings = menuSettings;
}

void ConfigScreen::setGameSettings(GameSettingsRef gameSettings)
{
	this->gameSettings = gameSettings;
}	

void ConfigScreen::startUpParams()
{
	changes.clear();

	for (auto setting : settingsList)
		setting->createMemento();

	mainConfig->setStartupData();

	gameSettingsScreen = nullptr;
}

void ConfigScreen::start()
{
	startUpParams();	
	mainConfig->connectEventHandler(&ConfigScreen::gamesBlockHandler, this);
	mainConfig->connectEventHandler(&ConfigScreen::showingMainConfAnimationComplete,  this, MainConfig::SHOW_ANIM_COMPLETE);
	mainConfig->startAnimation();
}

void ConfigScreen::stop()
{	
	mainConfig->disconnectEventHandler();
	mainConfig->unActivateListeners();
	photoboothConfig->unActivateListeners();
	instakubConfig->unActivateListeners();
}

void ConfigScreen::init()
{
	init(configSettings);
}

void ConfigScreen::init(ISettingsRef settings)
{
	configSettings			= static_pointer_cast<ConfigSettings>(settings);
	mainConfig				= MainConfigRef(new MainConfig(configSettings, gameSettings));
	addChild(mainConfig);

	PhotoboothSettingsRef phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(GameId::PHOTOBOOTH));
	photoboothConfig		= PhotoboothConfigRef(new PhotoboothConfig(phbthSettings));	
	
	InstakubSettingsRef instaSettings = static_pointer_cast<InstakubSettings>(gameSettings->get(GameId::INSTAKUB));
	instakubConfig		= InstakubConfigRef(new InstakubConfig(instaSettings));	

	settingsList.push_back(configSettings);
	settingsList.push_back(phbthSettings);
	settingsList.push_back(instaSettings);
}

////////////////////////////////////////////////////////////////////////////
//
//				DRAW
//
////////////////////////////////////////////////////////////////////////////

void ConfigScreen::draw()
{
	Sprite::draw();
}

void ConfigScreen::gamesBlockHandler(EventGUIRef& event)
{
	EventGUI *ev = event.get();

	if(!ev) return;

	if(typeid(*ev) == typeid(CloseConfigEvent))
	{
		mainConfig->unActivateListeners();
		closeLocationHandler();		
	}
	else if(typeid(*ev) == typeid(GameConfEvent))
	{
		GameConfEventRef confEvent = static_pointer_cast<GameConfEvent>(event);
		mainConfig->hideAnimate(confEvent->getGameId(), EaseOutCubic(), 0.7f);

		if (confEvent->getGameId() == GameId::PHOTOBOOTH)		
			gameSettingsScreen = photoboothConfig;	
		if (confEvent->getGameId() == GameId::INSTAKUB)		
			gameSettingsScreen = instakubConfig;	

		//gameSettingsScreen = gameSettings->get(confEvent->getGameId());
		gameSettingsScreen->setPosition(ci::Vec2f(1080.0f, 0.0f));
		gameSettingsScreen->showAnimate(ci::EaseOutCubic(), 0.7f);
		addChild(gameSettingsScreen);		

		console()<<"config game ID:::::: "<<confEvent->getGameId()<<endl;
	}
	else if(typeid(*ev) == typeid(BackToMainConfigEvent))
	{
		mainConfig->showAnimate(EaseOutCubic(), 0.7f);
		gameSettingsScreen->hideAnimate(EaseOutCubic(), 0.7f);
	}	
	if(typeid(*ev) == typeid(StatisticEvent))
	{
		StatisticEventRef statEvent = static_pointer_cast<StatisticEvent>(event);	
		console()<<"Statistic game ID:::::: "<<statEvent->getGameId()<<endl;
	}
	else if(typeid(*ev) == typeid(GameShowUrlEvent))
	{
		GameShowUrlEventRef urlEvent = static_pointer_cast<GameShowUrlEvent>(event);	
		console()<<"show url game ID:::::: "<<urlEvent->getGameId()<<endl;
	}	
	
	console()<<"EVENT:::::: "<<endl;	
}

void ConfigScreen::showingMainConfAnimationComplete()
{
	if(gameSettingsScreen)
	{
		removeChild(gameSettingsScreen);
		gameSettingsScreen = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////
//
//				CHECK CHANGES 
//
////////////////////////////////////////////////////////////////////////////

void ConfigScreen::closeLocationHandler()
{	
	for (auto setting : settingsList)
	{
		if(setting->settingsChanged())
		{
			setting->writeConfig();	
			Changes chng;
			chng.id = setting->getChangeID();
			changes.push_back(chng);	
		}
	}
	checkMenuParamsForChanges();
	checkScreenSaverParamsForChanges();
	checkGamesParamsForChanges();
	//check if design switched

	mainConfig->closeDesignBlock();
	closeLocationSignal();
}

void ConfigScreen::checkMenuParamsForChanges()
{	
	Changes chng;
	chng.id = changeSetting::id::MENU;

	//if(configSettings->settingsChanged())	
	//	changes.push_back(chng);	
}

void ConfigScreen::checkGamesParamsForChanges()
{
	/*if(gamesData.getCountSwitchOnGames())
	{
		Changes chng;
		chng.id = changeSetting::id::GAMES;	

		setDefaultGameIdInSwitchOnGames();
		setReloadGamePropertyIfNeedIt(chng);

		if (initialGamesData.getDefaultGameID() != gamesData.getDefaultGameID() || chng.gamesReload)
		{
			changes.push_back(chng);
			gameSettings->setData(gamesData);
		}
	}	
	*/
}

void ConfigScreen::checkScreenSaverParamsForChanges()
{
	/*if (screensaverData.hasChanges(initialScreensaverData))
	{
		Changes chng;
		chng.id = changeSetting::id::SCREENSAVER;	
		changes.push_back(chng);
		screenSaverSettings->setData(screensaverData);
	}*/
}

void ConfigScreen::setDefaultGameIdInSwitchOnGames()
{
	/*size_t len = gamesData.getGames().size();

	if(!gamesData.isIdInSwitchOnGames((GameId)gamesData.getDefaultGameID()))
	{
		for (size_t i = 0; i < len; i++)
		{
			if(gamesData.getGames()[i].isOn)
			{
				gamesData.setDefaultGameID(gamesData.getGameID(i));
				break;
			}
		}
	}*/
}

void ConfigScreen::setReloadGamePropertyIfNeedIt(Changes &chng)
{
	/*size_t len = gamesData.getGames().size();

	for (size_t i = 0; i < len; i++)
	{
		GamesInfo game = gamesData.getGames()[i];

		if( game.isPurchased && game.isOn != initialGamesData.getGames()[i].isOn)	
		{
			chng.gamesReload = true;
			break;			
		}	
	}*/
}