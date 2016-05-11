#include "ConfigScreen.h"
#include "Server/Server.h"

using namespace std;
using namespace params;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace kubik::config;
using namespace kubik;

ConfigScreen::ConfigScreen(ISettingsRef config) :IScreen(ScreenId::CONFIG)
{
	configSettings = static_pointer_cast<ConfigSettings>(config);
}

ConfigScreen::~ConfigScreen()
{
	console() << "~~~~~~~~~~~~~~~ ConfigScreen destructor ~~~~~~~~~~~~~~~" << endl;
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
	//server().standInfo();
	server().getSocialShareTexts();

	startUpParams();

	mainConfig->connectEventHandler(&ConfigScreen::gamesBlockHandler, this);
	mainConfig->connectEventHandler(&ConfigScreen::showingMainConfAnimationComplete, this, MainConfig::SHOW_ANIM_COMPLETE);
	mainConfig->startAnimation();

	initShowAnimation();
}

void ConfigScreen::initShowAnimation()
{
	//screenshot = getScreenShot();
	state = SHOW_ANIM;
	timeline().apply(&animX, 1080.0f, 0.0f, 0.9f, EaseOutCubic()).finishFn(bind(&ConfigScreen::showAnimationComplete, this));
	timeline().apply(&animX1, 0.0f, -500.0f, 0.9f, EaseOutCubic());
	timeline().apply(&alpha, 1.0f, 0.2f, 0.9f, EaseOutCubic());
}

void ConfigScreen::showAnimationComplete()
{
	state = DRAW;
}

void ConfigScreen::stop()
{
	mainConfig->disconnectEventHandler();
	mainConfig->unActivateListeners();
	mainConfig->killAll();

	console() << "::::::::::: exit   " << gameSettings->has(GameId::PHOTOBOOTH) << endl;

	if (photoboothConfig)//gameSettings->has(GameId::PHOTOBOOTH))
		photoboothConfig->unActivateListeners();

	if (gameSettings->has(GameId::INSTAKUB))
		instakubConfig->unActivateListeners();
}

void ConfigScreen::init()
{
	init(configSettings);
}

void ConfigScreen::init(ISettingsRef settings)
{
	configSettings = static_pointer_cast<ConfigSettings>(settings);
	mainConfig = MainConfigRef(new MainConfig(configSettings, gameSettings, screenSaverSettings));
	addChild(mainConfig);
	
	console() << ":::::::::::   " << gameSettings->has(GameId::PHOTOBOOTH) << endl;
	if (gameSettings->has(GameId::PHOTOBOOTH))
	{
		PhotoboothSettingsRef phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(GameId::PHOTOBOOTH));
		photoboothConfig = PhotoboothConfigRef(new PhotoboothConfig(phbthSettings));
		settingsList.push_back(phbthSettings);
	}
	
	if (gameSettings->has(GameId::INSTAKUB))
	{
		InstakubSettingsRef instaSettings = static_pointer_cast<InstakubSettings>(gameSettings->get(GameId::INSTAKUB));
		instakubConfig = InstakubConfigRef(new InstakubConfig(instaSettings));
		settingsList.push_back(instaSettings);
	}	

	settingsList.push_back(screenSaverSettings);
	settingsList.push_back(gameSettings);
	settingsList.push_back(configSettings);
}

////////////////////////////////////////////////////////////////////////////
//
//				DRAW
//
////////////////////////////////////////////////////////////////////////////

void ConfigScreen::draw()
{
	switch (state)
	{
	case SHOW_ANIM:
		gl::pushMatrices();
		gl::translate(animX1, 0.0f);
		gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
		drawScreenShot();
		gl::color(Color::white());
		gl::popMatrices();

		gl::pushMatrices();
		gl::translate(animX, 0.0f);
		Sprite::draw();
		gl::popMatrices();
		break;

	case DRAW:
		Sprite::draw();
		break;
	}
}

void ConfigScreen::gamesBlockHandler(EventGUIRef& event)
{
	EventGUI *ev = event.get();

	if (!ev) return;

	if (typeid(*ev) == typeid(CloseConfigEvent))
	{
		mainConfig->unActivateListeners();
		closeLocationHandler();
	}
	else if (typeid(*ev) == typeid(GameConfEvent))
	{
		GameConfEventRef confEvent = static_pointer_cast<GameConfEvent>(event);

		if (confEvent->getGameId() == GameId::PHOTOBOOTH)		
			gameSettingsScreen = photoboothConfig;		
		else if (confEvent->getGameId() == GameId::INSTAKUB)
			gameSettingsScreen = instakubConfig;
		else
			return;
		
		mainConfig->hideAnimate(confEvent->getGameId(), EaseOutCubic(), 0.7f);		

		gameSettingsScreen->setPosition(ci::Vec2f(1080.0f, 0.0f));
		gameSettingsScreen->showAnimate(ci::EaseOutCubic(), 0.7f);
		addChild(gameSettingsScreen);
	}
	else if (typeid(*ev) == typeid(BackToMainConfigEvent))
	{
		if (gameSettingsScreen->canClose())
		{
			mainConfig->showAnimate(EaseOutCubic(), 0.7f);
			gameSettingsScreen->hideAnimate(EaseOutCubic(), 0.7f);
		}
		else
		{
			gameSettingsScreen->showErrorPopup();
		}
	}
	if (typeid(*ev) == typeid(StatisticEvent))
	{
		StatisticEventRef statEvent = static_pointer_cast<StatisticEvent>(event);
		console() << "Statistic game ID:::::: " << statEvent->getGameId() << endl;
	}
	else if (typeid(*ev) == typeid(GameShowUrlEvent))
	{
		GameShowUrlEventRef urlEvent = static_pointer_cast<GameShowUrlEvent>(event);
		console() << "show url game ID:::::: " << urlEvent->getGameId() << endl;
	}

	//console() << "EVENT:::::: " << endl;
}

void ConfigScreen::showingMainConfAnimationComplete()
{
	if (gameSettingsScreen)
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
		if (setting->settingsChanged())
		{
			setting->writeConfig();
			Changes chng;
			chng.id = setting->getChangeID();
			changes.push_back(chng);
		}
	}

	mainConfig->closeDesignBlock();

	bool reloadDesign = configSettings->designChanged();

	if (!changes.size())
		closeLocationSignal();
	else
		appSettingsChangedSignal(changes, reloadDesign);
}