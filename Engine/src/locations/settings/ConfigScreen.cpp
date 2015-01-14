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

	mouseUpListener.disconnect();
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
	phData	   = phbthSettings->getData();
	initPhData = phbthSettings->getData();

	menuData = menuSettings->getData();
	initialMenuData = menuSettings->getData();

	gamesData = gameSettings->getData();
	initialGamesData = gameSettings->getData();

	screensaverData		   = screenSaverSettings->getData();
	initialScreensaverData = screenSaverSettings->getData();



	console()<<"STARTUP!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	statBlock->setPlayedTimes(configSettings->getData().playedCount);
	statBlock->setPrintedPhotos(configSettings->getData().printedCount);
	statBlock->setSharedAndEmail(configSettings->getData().puplishedCount);

	printerBlock->setÑurrentPhotosPrinted(configSettings->getData().currentPhotosPrinted);	

	title->setActivityName("Ïðîìîàêöèÿ");
}

void ConfigScreen::start()
{
	startUpParams();
	addMouseUpListener();
}

void ConfigScreen::stop()
{
	removeMouseUpListener();

	for(auto param:params)
		param->hide();
}

void ConfigScreen::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&ConfigScreen::mouseUp, this);
	closeBlock->addMouseUpListener();
	startNewActivity->addMouseUpListener();
	menuBlock->addMouseUpListener();
	screenSaverBlock->addMouseUpListener();
	printerBlock->addMouseUpListener();	
}

void ConfigScreen::mouseUp(MouseEvent &event)
{
	//closeBtn->mouseUpHandler(event.getPos());
	//saveChngBtn->mouseUpHandler(event.getPos());	
}

void ConfigScreen::removeMouseUpListener()
{
	mouseUpListener.disconnect();
	closeBlock->removeMouseUpListener();
	startNewActivity->removeMouseUpListener();
	menuBlock->removeMouseUpListener();
	screenSaverBlock->removeMouseUpListener();
	printerBlock->removeMouseUpListener();	
}

void ConfigScreen::init()
{
	init(configSettings);
}

void ConfigScreen::init(ISettingsRef settings)
{
	configSettings	= static_pointer_cast<ConfigSettings>(settings);

	font					= configSettings->getFont("helvetica90");
	Font fontBtn			= configSettings->getFont("helvetica20");
	Font introLight44		= configSettings->getFont("introLight44");
	Font helveticaLight22	= configSettings->getFont("helveticaLight22");
	Font helveticaLight24	= configSettings->getFont("helveticaLight24");
	Font introBold110		= configSettings->getFont("introBold110");
	Font introBold72		= configSettings->getFont("introBold72");

	//saveChngBtn = ButtonTextRef(new ButtonText(Rectf(1200.0f, 650.0f, 1350.0f, 750.0f), "Ñîõðàíèòü", fontBtn));	
	//appSettingsChgListener = saveChngBtn->mouseUpSignal.connect(bind(&ConfigScreen::appSettingsChgHandler, this, std::placeholders::_1));
	//tempBg = configSettings->getTexture("tempBg");

	tempBg = configSettings->getTexture("tempBottom");

	closeBlock = CloseBlockRef(new CloseBlock());
	closeBlock->setPosition(Vec2i(916, 66));
	closeBlock->setCloseTexture(configSettings->getTexture("closeImg"));
	closeBlock->createBtn();
	connect_once(closeBlock->closeSignal, bind(&ConfigScreen::closeLocationHandler, this, std::placeholders::_1));

	title = TitleRef(new Title());
	title->setPosition(Vec2i(100, 60));
	title->setFont(introLight44);

	startNewActivity = StartNewActivityRef(new StartNewActivity());
	startNewActivity->setPosition(Vec2i(96, 137));
	startNewActivity->setIcon(configSettings->getTexture("iconStartNew"));
	startNewActivity->setFont(helveticaLight24);
	startNewActivity->createBtn();
	//connect_once(startNewActivity->startNewActivitySignal, bind(&ConfigScreen::startNewActivityHandler, this, std::placeholders::_1));
	//connect_once(startNewActivity->cancelNewActivityTrySignal, bind(&ConfigScreen::, this, std::placeholders::_1));
	//connect_once(startNewActivity->tryToStartNewActivitySignal, bind(&ConfigScreen::, this, std::placeholders::_1));

	statBlock = StatBlockRef(new StatBlock());
	statBlock->setPosition(Vec2i(100, 235));
	statBlock->setTitleFont(helveticaLight22);
	statBlock->setNumsFont(introBold110);

	menuBlock = MenuBlockRef(new MenuBlock());
	menuBlock->setPosition(Vec2i(100, 424));
	menuBlock->setTitleFont(introLight44);
	menuBlock->setSubTitleFont(helveticaLight22);
	menuBlock->setIcon(configSettings->getTexture("menuIcon"));
	menuBlock->createBtn();
	//connect_once(menuBlock->..., bind(&ConfigScreen::, this, std::placeholders::_1));

	screenSaverBlock = ScreenSaverBlockRef(new ScreenSaverBlock());
	screenSaverBlock->setPosition(Vec2i(533, 424));
	screenSaverBlock->setTitleFont(introLight44);
	screenSaverBlock->setSubTitleFont(helveticaLight22);
	screenSaverBlock->setIcon(configSettings->getTexture("ssIcon"));
	screenSaverBlock->createBtn();
	//connect_once(screenSaverBlock->..., bind(&ConfigScreen::, this, std::placeholders::_1));
	
	gamesBlock = GamesBlockRef(new GamesBlock());
	gamesBlock->setPosition(Vec2i(100, 602));

	printerBlock = PrinterBlockRef(new PrinterBlock());
	printerBlock->setPosition(Vec2i(0, getWindowHeight() - 170.0f));
	printerBlock->setNumsFont(introBold72);
	printerBlock->setHintFont(helveticaLight24);
	printerBlock->setChangeBtnFont(helveticaLight24);
	printerBlock->setChangeBtnIcon(configSettings->getTexture("catridgeIcon"));
	printerBlock->setMaxPhotosToPrint(configSettings->getData().maxPhotosToPrint);
	printerBlock->createBtn();
	//connect_once(printerBlock->..., bind(&ConfigScreen::, this, std::placeholders::_1));

	logo = LogoRef(new Logo());
	logo->setIcon(configSettings->getTexture("logoIcon"));
	logo->setPosition(Vec2i(835, getWindowHeight() - 170.0f));

	components.clear();
	components.push_back(title);
	components.push_back(startNewActivity);
	components.push_back(statBlock);
	components.push_back(menuBlock);
	components.push_back(closeBlock);
	components.push_back(screenSaverBlock);
	components.push_back(gamesBlock);
	components.push_back(printerBlock);
	components.push_back(logo);

	createPhotoboothParams();
	createFuncesParams();
	createMenuParams();	
	createGamesParams();
	createScreensaverParams();
}

////////////////////////////////////////////////////////////////////////////
//
//				DRAW
//
////////////////////////////////////////////////////////////////////////////

void ConfigScreen::draw()
{
	gl::color(Color::hex(0x0d0917));
	gl::drawSolidRect(getWindowBounds());
	
	gl::color(ColorA(1, 1, 1, 0.5f));	
	gl::draw(tempBg, Vec2f(0, getWindowHeight() - tempBg.getHeight()));
	gl::color(Color::white());

	for (auto comp : components)
		comp->draw();	

	//for(auto param: params)
	//	param->draw();
}

void ConfigScreen::closeLocationHandler(IButton& button )
{	
	closeLocationSignal();
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATING PARAMS
//
////////////////////////////////////////////////////////////////////////////

void ConfigScreen::createPhotoboothParams()
{	
	shared_ptr<PhotoboothSettings> phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(game::id::PHOTOBOOTH));	
	phData = phbthSettings->getData();

	photoBoothParams = InterfaceGl::create(getWindow(), "Photobooth parameters", toPixels(Vec2i(300, 400)));

	photoBoothParams->addParam("seconds", &phData.seconds)
		.min((float)phData.MIN_COUNTDOWN_TIMER)
		.max((float)phData.MAX_COUNTDOWN_TIMER)
		.step(1);
	photoBoothParams->addParam("secondsBetweenShots", &phData.secondsBetweenShots)
		.min((float)phData.MIN_SEC_BETWEEN_SHOTS)
		.max((float)phData.MAX_SEC_BETWEEN_SHOTS)
		.step(1);
	photoBoothParams->addParam("photoNum", &phData.photoNum)
		.min((float)phData.MIN_PHOTO_SHOTS)
		.max((float)phData.MAX_PHOTO_SHOTS) 
		.step(2);
	photoBoothParams->addSeparator();
	photoBoothParams->addParam("Custom design",		&phData.isCustomDesign);
	photoBoothParams->addParam("Template id",		&phData.templateId).min(1).max(2).step(1);
	photoBoothParams->addButton("Show In Explorer1", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
	photoBoothParams->addSeparator();
	
	photoBoothParams->addParam("isSticker",			&phData.isSticker);
	photoBoothParams->addParam("Sticker ids",		&phData.activeSticker.id).max((float)phData.stickers.size()-1).min(0).step(1);
	photoBoothParams->addButton("Show In Explorer2", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
	photoBoothParams->addSeparator();
	photoBoothParams->addParam("Bg Print ids",		&phData.activeBgPrint.id).max((float)phData.bgPrint.size()-1).min(0).step(1);
	photoBoothParams->addButton("Show In Explorer3", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
	photoBoothParams->addSeparator();
	photoBoothParams->addParam("Facebook Sharing",  &phData.sharing.isFacebook);
	photoBoothParams->addParam("Vkontakte Sharing", &phData.sharing.isVkotakte);
	photoBoothParams->addParam("Twitter Sharing",   &phData.sharing.isTwitter);
	photoBoothParams->addParam("Printer",			&phData.sharing.isPrint);
	photoBoothParams->addParam("Email",				&phData.sharing.isEmail);
	photoBoothParams->addParam("QrCode",			&phData.sharing.isQrCode);
	photoBoothParams->hide();

	params.push_back(photoBoothParams);
}

void ConfigScreen::createFuncesParams()
{

}

void ConfigScreen::createMenuParams()
{
	menuData = menuSettings->getData();	
	menuParams = InterfaceGl::create(getWindow(), "Menu parameters", toPixels(Vec2i(300, 200)));	
	menuParams->setPosition(Vec2i(350, 20));
	menuParams->addParam("Custom design",	 &menuData.isCustomDesign);
	menuParams->addParam("Template id",		 &menuData.templateId).min(1).max(2).step(1);	
	menuParams->addButton("Show In Explorer", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
	menuParams->addSeparator();	
	menuParams->hide();

	params.push_back(menuParams);
}

void ConfigScreen::showInExplorerMenuDesignPath()
{
	if(menuData.isCustomDesign)	
		showInExplorer(menuSettings->getUserDesighFullPath());
	else
	   showInExplorer(menuSettings->getKubikDesighFullPath());
}

void ConfigScreen::showInExplorer(string path)
{
	//system("start explorer c:\\");
	std::wstring stemp = stringTools().s2ws(path);	
	console()<<"show in explorer::: "<<path<<endl;

	ShellExecute(NULL, L"open", (LPCWSTR)path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

void ConfigScreen::createGamesParams()
{
	gamesData = gameSettings->getData();
	gamesParams = InterfaceGl::create(getWindow(), "Games parameters", toPixels(Vec2i(300, 200)));
	gamesParams->setPosition(Vec2i(690, 20));
	//gamesParams->addText("Action Name -------------    " + stringTools().cp1251_to_utf8(gameSettings->getData().actionName.c_str()));
	gamesParams->addSeparator();	
	for (auto it = gamesData.games.begin(); it < gamesData.games.end(); it++)
	{
		if( it->isPurchased)		
			gamesParams->addParam(it->name, &it->isOn);
	}

	gamesParams->addSeparator();	
	gamesParams->addParam("Default Game ID", &gamesData.defaultGameID).min(1).max(2).step(1);
	gamesParams->hide();

	params.push_back(gamesParams);
}

void ConfigScreen::createScreensaverParams()
{
	screensaverData = screenSaverSettings->getData();

	screensaverParams = InterfaceGl::create(getWindow(), "Screensaver parameters", toPixels(Vec2i(300, 60)));
	screensaverParams->setPosition(Vec2i(690, 250));
	screensaverParams->addParam("Is Active", &screensaverData.isActive);
	screensaverParams->addButton("Show In Explorer", bind(&ConfigScreen::showInExplorerMenuDesignPath, this));
	screensaverParams->hide();

	params.push_back(screensaverParams);
}

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
	shared_ptr<PhotoboothSettings> phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(game::id::PHOTOBOOTH));
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

		if (initialGamesData.defaultGameID != gamesData.defaultGameID || chng.gamesReload)
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
	size_t len = gamesData.games.size();

	if(!gamesData.isIdInSwitchOnGames((game::id)gamesData.defaultGameID))
	{
		for (size_t i = 0; i < len; i++)
		{
			if(gamesData.games[i].isOn)
			{
				gamesData.defaultGameID = gamesData.games[i].id;
				break;
			}
		}
	}
}

void ConfigScreen::setReloadGamePropertyIfNeedIt(Changes &chng)
{
	size_t len = gamesData.games.size();

	for (size_t i = 0; i < len; i++)
	{
		GamesInfo game = gamesData.games[i];

		if( game.isPurchased && game.isOn != initialGamesData.games[i].isOn)	
		{
			chng.gamesReload = true;
			break;			
		}	
	}
}
