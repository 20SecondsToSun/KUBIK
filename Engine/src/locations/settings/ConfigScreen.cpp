#include "ConfigScreen.h"
#include <shellapi.h>
//#pragma comment(lib, "shell32")

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

	PhotoboothSettingsRef phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(GameId::PHOTOBOOTH));
	initPhData = phData = phbthSettings->getData();	
	initialMenuData = menuData = menuSettings->getData();
	initialGamesData = gamesData = gameSettings->getData();
	initialScreensaverData = screensaverData = screenSaverSettings->getData();

	console()<<"STARTUP!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	mainConfig->setStartupData();
	photoboothConfig->setInitPosition();
}

void ConfigScreen::start()
{
	startUpParams();	
	mainConfig->addMouseUpListener(&ConfigScreen::gamesBlockHandler, this);
	photoboothConfig->activateListeners();
}

void ConfigScreen::stop()
{	
	mainConfig->removeMouseUpListener();
	photoboothConfig->unActivateListeners();
}

void ConfigScreen::init()
{
	init(configSettings);
}

void ConfigScreen::init(ISettingsRef settings)
{
	configSettings			= static_pointer_cast<ConfigSettings>(settings);
	mainConfig				= MainConfigRef(new MainConfig(configSettings, gameSettings));

	PhotoboothSettingsRef phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(GameId::PHOTOBOOTH));
	photoboothConfig		= PhotoboothConfigRef(new PhotoboothConfig(phbthSettings));	
}

////////////////////////////////////////////////////////////////////////////
//
//				DRAW
//
////////////////////////////////////////////////////////////////////////////

void ConfigScreen::draw()
{
	//mainConfig->draw();
	
	//gl::color(ColorA(1.0f, 1.0f, 1.0f, 0.5f));	
	Texture tempBg = configSettings->getTexture("temp");
	//gl::draw(tempBg, Vec2f(0.0f, getWindowHeight() - tempBg.getHeight()));
	//tempBg = configSettings->getTexture("appsTemp");
	//gl::draw(tempBg);//,  Vec2f(1080.0f, 10.0f - 500));
	gl::color(ColorA(1.0f, 1.0f, 1.0f, 0.5f));	
	photoboothConfig->draw();
	//gl::color(Color::white());	
}

void ConfigScreen::closeLocationHandler(EventRef& event)
{	
	closeLocationSignal();
}

void ConfigScreen::gamesBlockHandler(EventRef& event)
{
	Event *ev = event.get();

	if(typeid(*ev) == typeid(StatisticEvent))
	{
		StatisticEventRef statEvent = static_pointer_cast<StatisticEvent>(event);	
		console()<<"Statistic game ID:::::: "<<statEvent->getGameId()<<endl;
	}
	else if(typeid(*ev) == typeid(GameConfEvent))
	{
		GameConfEventRef statEvent = static_pointer_cast<GameConfEvent>(event);	
		console()<<"config game ID:::::: "<<statEvent->getGameId()<<endl;
	}
	else if(typeid(*ev) == typeid(GameShowUrlEvent))
	{
		GameShowUrlEventRef urlEvent = static_pointer_cast<GameShowUrlEvent>(event);	
		console()<<"show url game ID:::::: "<<urlEvent->getGameId()<<endl;
	}
	else if(typeid(*ev) == typeid(CloseConfigEvent))
	{
		console()<<"close location:::::: "<<endl;
		closeLocationSignal();
	}	
	console()<<"EVENT:::::: "<<event->getMsg()<<endl;
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATING PARAMS
//
////////////////////////////////////////////////////////////////////////////
//
//void ConfigScreen::createPhotoboothParams()
//{	
//	shared_ptr<PhotoboothSettings> phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(game::id::PHOTOBOOTH));	
//	phData = phbthSettings->getData();
//
//	photoBoothParams = InterfaceGl::create(getWindow(), "Photobooth parameters", toPixels(Vec2i(300, 400)));
//
//	photoBoothParams->addParam("seconds", &phData.seconds)
//		.min((float)phData.MIN_COUNTDOWN_TIMER)
//		.max((float)phData.MAX_COUNTDOWN_TIMER)
//		.step(1);
//	photoBoothParams->addParam("secondsBetweenShots", &phData.secondsBetweenShots)
//		.min((float)phData.MIN_SEC_BETWEEN_SHOTS)
//		.max((float)phData.MAX_SEC_BETWEEN_SHOTS)
//		.step(1);
//	photoBoothParams->addParam("photoNum", &phData.photoNum)
//		.min((float)phData.MIN_PHOTO_SHOTS)
//		.max((float)phData.MAX_PHOTO_SHOTS) 
//		.step(2);
//	photoBoothParams->addSeparator();
//	photoBoothParams->addParam("Custom design",		&phData.isCustomDesign);
//	photoBoothParams->addParam("Template id",		&phData.templateId).min(1).max(2).step(1);
//	photoBoothParams->addButton("Show In Explorer1", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
//	photoBoothParams->addSeparator();
//	
//	photoBoothParams->addParam("isSticker",			&phData.isSticker);
//	photoBoothParams->addParam("Sticker ids",		&phData.activeSticker.id).max((float)phData.stickers.size()-1).min(0).step(1);
//	photoBoothParams->addButton("Show In Explorer2", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
//	photoBoothParams->addSeparator();
//	photoBoothParams->addParam("Bg Print ids",		&phData.activeBgPrint.id).max((float)phData.bgPrint.size()-1).min(0).step(1);
//	photoBoothParams->addButton("Show In Explorer3", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
//	photoBoothParams->addSeparator();
//	photoBoothParams->addParam("Facebook Sharing",  &phData.sharing.isFacebook);
//	photoBoothParams->addParam("Vkontakte Sharing", &phData.sharing.isVkotakte);
//	photoBoothParams->addParam("Twitter Sharing",   &phData.sharing.isTwitter);
//	photoBoothParams->addParam("Printer",			&phData.sharing.isPrint);
//	photoBoothParams->addParam("Email",				&phData.sharing.isEmail);
//	photoBoothParams->addParam("QrCode",			&phData.sharing.isQrCode);
//	photoBoothParams->hide();
//
//	params.push_back(photoBoothParams);
//}
//
//void ConfigScreen::createFuncesParams()
//{
//
//}
//
//void ConfigScreen::createMenuParams()
//{
//	menuData = menuSettings->getData();	
//	menuParams = InterfaceGl::create(getWindow(), "Menu parameters", toPixels(Vec2i(300, 200)));	
//	menuParams->setPosition(Vec2i(350, 20));
//	menuParams->addParam("Custom design",	 &menuData.isCustomDesign);
//	menuParams->addParam("Template id",		 &menuData.templateId).min(1).max(2).step(1);	
//	menuParams->addButton("Show In Explorer", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
//	menuParams->addSeparator();	
//	menuParams->hide();
//
//	params.push_back(menuParams);
//}
//
//void ConfigScreen::showInExplorerMenuDesignPath()
//{
//	if(menuData.isCustomDesign)	
//		showInExplorer(menuSettings->getUserDesighFullPath());
//	else
//	   showInExplorer(menuSettings->getKubikDesighFullPath());
//}
//
//void ConfigScreen::showInExplorer(string path)
//{
//	//system("start explorer c:\\");
//	std::wstring stemp = stringTools().s2ws(path);	
//	console()<<"show in explorer::: "<<path<<endl;
//
//	ShellExecute(NULL, L"open", (LPCWSTR)path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
//}
//
//void ConfigScreen::createGamesParams()
//{
//	gamesData = gameSettings->getData();
//	gamesParams = InterfaceGl::create(getWindow(), "Games parameters", toPixels(Vec2i(300, 200)));
//	gamesParams->setPosition(Vec2i(690, 20));
//	//gamesParams->addText("Action Name -------------    " + stringTools().cp1251_to_utf8(gameSettings->getData().actionName.c_str()));
//	gamesParams->addSeparator();	
//	for (auto it = gamesData.games.begin(); it < gamesData.games.end(); it++)
//	{
//		if( it->isPurchased)		
//			gamesParams->addParam(it->name, &it->isOn);
//	}
//
//	gamesParams->addSeparator();	
//	gamesParams->addParam("Default Game ID", &gamesData.defaultGameID).min(1).max(2).step(1);
//	gamesParams->hide();
//
//	params.push_back(gamesParams);
//}
//
//void ConfigScreen::createScreensaverParams()
//{
//	screensaverData = screenSaverSettings->getData();
//
//	screensaverParams = InterfaceGl::create(getWindow(), "Screensaver parameters", toPixels(Vec2i(300, 60)));
//	screensaverParams->setPosition(Vec2i(690, 250));
//	screensaverParams->addParam("Is Active", &screensaverData.isActive);
//	screensaverParams->addButton("Show In Explorer", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
//	screensaverParams->hide();
//
//	params.push_back(screensaverParams);
//}

////////////////////////////////////////////////////////////////////////////
//
//				CHECK CHANGES 
//
////////////////////////////////////////////////////////////////////////////

void ConfigScreen::appSettingsChgHandler(ButtonText& button )
{
	checkPhotoBoothParamsForChanges();
	checkMenuParamsForChanges();
	checkGamesParamsForChanges();
	checkScreenSaverParamsForChanges();

	if(changes.size())
		appSettingsChangedSignal(changes);
	else 
		closeLocationSignal();
}

void ConfigScreen::checkPhotoBoothParamsForChanges()
{
	PhotoboothSettingsRef phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(game::id::PHOTOBOOTH));
	Changes chng;
	chng.id = changeSetting::id::PHOTOBOOTH;

	if(phData.hasDesignChanges(initPhData))
	{
		chng.texReload = true;
		changes.push_back(chng);
		phbthSettings->setData(phData);	
		phbthSettings->setDesignPath();
	}
	else if(phData.hasAnyChanges(initPhData))	
	{
		changes.push_back(chng);
		phbthSettings->setData(phData);	
	}
}

void ConfigScreen::checkFuncesParamsForChanges()
{
	//Changes chng;
	//chng.id = ChangeSettingID::FUNCES;
}

void ConfigScreen::checkMenuParamsForChanges()
{	
	if (menuData.hasDesignChanges(initialMenuData))
	{
		Changes chng;
		chng.id = changeSetting::id::MENU;
		chng.texReload = true;
		changes.push_back(chng);
		menuSettings->setData(menuData);	
		menuSettings->setDesignPath();	
	}	
}

void ConfigScreen::checkGamesParamsForChanges()
{
	if(gamesData.getCountSwitchOnGames())
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
}

void ConfigScreen::checkScreenSaverParamsForChanges()
{
	if (screensaverData.hasChanges(initialScreensaverData))
	{
		Changes chng;
		chng.id = changeSetting::id::SCREENSAVER;	
		changes.push_back(chng);
		screenSaverSettings->setData(screensaverData);
	}
}

void ConfigScreen::setDefaultGameIdInSwitchOnGames()
{
	size_t len = gamesData.getGames().size();

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
	}
}

void ConfigScreen::setReloadGamePropertyIfNeedIt(Changes &chng)
{
	size_t len = gamesData.getGames().size();

	for (size_t i = 0; i < len; i++)
	{
		GamesInfo game = gamesData.getGames()[i];

		if( game.isPurchased && game.isOn != initialGamesData.getGames()[i].isOn)	
		{
			chng.gamesReload = true;
			break;			
		}	
	}
}