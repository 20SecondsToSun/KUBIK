#include "TuneUpScreen.h"
#include <shellapi.h>
//#pragma comment(lib, "shell32")

using namespace kubik;

TuneUpScreen::TuneUpScreen(shared_ptr<TuneUpSettings>	   config,
						   shared_ptr<ScreenSaverSettings> screenSaverSettings,
						   shared_ptr<MenuSettings>        menuConfig,
						   shared_ptr<GameSettings>		   gameSettings)
{	
	this->menuSettings		  = menuConfig;
	this->gameSettings		  = gameSettings;
	this->screenSaverSettings = screenSaverSettings;
	init(config);
}

TuneUpScreen::~TuneUpScreen()
{	
	console()<<"~~~~~~~~~~~~~~~ TuneUpScreen destructor ~~~~~~~~~~~~~~~"<<endl;

	mouseUpListener.disconnect();
	closeBtnListener.disconnect();	
	appSettingsChgListener.disconnect();	
}

void TuneUpScreen::startUpParams()
{
	changes.clear();

	shared_ptr<PhotoboothSettings> phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(game::id::PHOTOBOOTH));
	phData	   = phbthSettings->getData();
	initPhData = phbthSettings->getData();

	menuData = menuSettings->getData();
	initialMenuData = menuSettings->getData();

	gamesData = gameSettings->getData();
	initialGamesData = gameSettings->getData();

	screensaverData		   = screenSaverSettings->getData();
	initialScreensaverData = screenSaverSettings->getData();
}

void TuneUpScreen::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&TuneUpScreen::mouseUp, this);

	for(auto param:params)
		param->show();
}

void TuneUpScreen::mouseUp(MouseEvent &event)
{
	closeBtn->mouseUpHandler(event.getPos());
	saveChngBtn->mouseUpHandler(event.getPos());	
}

void TuneUpScreen::removeMouseUpListener()
{
	mouseUpListener.disconnect();

	for(auto param:params)
		param->hide();
}

void TuneUpScreen::init(shared_ptr<ISettings> settings)
{
	console()<<"set settings screen"<<endl;

	tuneUpSettings	= static_pointer_cast<TuneUpSettings>(settings);

	font			=  tuneUpSettings->getFonts()["helvetica90"]->get();
	Font fontBtn	=  tuneUpSettings->getFonts()["helvetica20"]->get();

	saveChngBtn = shared_ptr<ButtonText>(new ButtonText(Rectf(1200.0f, 650.0f, 1350.0f, 750.0f), "Ñîõðàíèòü", fontBtn));	
	appSettingsChgListener = saveChngBtn->mouseUpSignal.connect(bind(&TuneUpScreen::appSettingsChgHandler, this, std::placeholders::_1));

	Texture closeImg = tuneUpSettings->getTextures()["closeImg"]->get();
	closeBtn = shared_ptr<Button>(new Button(closeImg, Vec2f(getWindowWidth() - 100.0f, 100.0f)));		
	closeBtnListener = closeBtn->mouseUpSignal.connect(bind(&TuneUpScreen::closeLocationHandler, this, std::placeholders::_1));

	createPhotoboothParams();
	createFuncesParams();
	createMenuParams();	
	createGamesParams();
	createScreensaverParams();
}

void TuneUpScreen::closeLocationHandler(Button& button )
{	
	closeLocationSignal();
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATING PARAMS
//
////////////////////////////////////////////////////////////////////////////

void TuneUpScreen::createPhotoboothParams()
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
	photoBoothParams->addButton("Show In Explorer1", bind(&TuneUpScreen::showInExplorerMenuDesignPath, this));
	photoBoothParams->addSeparator();
	
	photoBoothParams->addParam("isSticker",			&phData.isSticker);
	photoBoothParams->addParam("Sticker ids",		&phData.activeSticker.id).max((float)phData.stickers.size()-1).min(0).step(1);
	photoBoothParams->addButton("Show In Explorer2", bind(&TuneUpScreen::showInExplorerMenuDesignPath, this));
	photoBoothParams->addSeparator();
	photoBoothParams->addParam("Bg Print ids",		&phData.activeBgPrint.id).max((float)phData.bgPrint.size()-1).min(0).step(1);
	photoBoothParams->addButton("Show In Explorer3", bind(&TuneUpScreen::showInExplorerMenuDesignPath, this));
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

void TuneUpScreen::createFuncesParams()
{

}

void TuneUpScreen::createMenuParams()
{
	menuData = menuSettings->getData();	
	menuParams = InterfaceGl::create(getWindow(), "Menu parameters", toPixels(Vec2i(300, 200)));	
	menuParams->setPosition(Vec2i(350, 20));
	menuParams->addParam("Custom design",	 &menuData.isCustomDesign);
	menuParams->addParam("Template id",		 &menuData.templateId).min(1).max(2).step(1);	
	menuParams->addButton("Show In Explorer", bind(&TuneUpScreen::showInExplorerMenuDesignPath, this));
	menuParams->addSeparator();	
	menuParams->hide();

	params.push_back(menuParams);
}

void TuneUpScreen::showInExplorerMenuDesignPath()
{
	if(menuData.isCustomDesign)	
		showInExplorer(menuSettings->getUserDesighFullPath());
	else
	   showInExplorer(menuSettings->getKubikDesighFullPath());
}

void TuneUpScreen::showInExplorer(string path)
{
	//system("start explorer c:\\");
	std::wstring stemp = stringTools().s2ws(path);	
	console()<<"show in explorer::: "<<path<<endl;

	ShellExecute(NULL, L"open", (LPCWSTR)path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

void TuneUpScreen::createGamesParams()
{
	gamesData = gameSettings->getData();
	gamesParams = InterfaceGl::create(getWindow(), "Games parameters", toPixels(Vec2i(300, 200)));
	gamesParams->setPosition(Vec2i(690, 20));
	gamesParams->addText("Action Name -------------    " + stringTools().cp1251_to_utf8(gameSettings->getData().actionName.c_str()));
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

void TuneUpScreen::createScreensaverParams()
{
	screensaverData = screenSaverSettings->getData();

	screensaverParams = InterfaceGl::create(getWindow(), "Screensaver parameters", toPixels(Vec2i(300, 60)));
	screensaverParams->setPosition(Vec2i(690, 250));
	screensaverParams->addParam("Is Active", &screensaverData.isActive);
	screensaverParams->addButton("Show In Explorer", bind(&TuneUpScreen::showInExplorerMenuDesignPath, this));
	screensaverParams->hide();

	params.push_back(screensaverParams);
}

////////////////////////////////////////////////////////////////////////////
//
//				CHECK CHANGES 
//
////////////////////////////////////////////////////////////////////////////

void TuneUpScreen::appSettingsChgHandler(ButtonText& button )
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

void TuneUpScreen::checkPhotoBoothParamsForChanges()
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

void TuneUpScreen::checkFuncesParamsForChanges()
{
	//Changes chng;
	//chng.id = ChangeSettingID::FUNCES;
}

void TuneUpScreen::checkMenuParamsForChanges()
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

void TuneUpScreen::checkGamesParamsForChanges()
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

void TuneUpScreen::checkScreenSaverParamsForChanges()
{
	if (screensaverData.hasChanges(initialScreensaverData))
	{
		Changes chng;
		chng.id = changeSetting::id::SCREENSAVER;	
		changes.push_back(chng);
		screenSaverSettings->setData(screensaverData);
	}
}

void TuneUpScreen::setDefaultGameIdInSwitchOnGames()
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

void TuneUpScreen::setReloadGamePropertyIfNeedIt(Changes &chng)
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

////////////////////////////////////////////////////////////////////////////
//
//				DRAW
//
////////////////////////////////////////////////////////////////////////////

void TuneUpScreen::draw()
{
	gl::color(Color(1, 0, 0));
	gl::drawSolidRect(getWindowBounds());

	gl::color(Color::white());
	//textTools().textFieldDraw("ÍÀÑÒÐÎÉÊÈ", &font, Vec2i(100, 100), Color::white());

	saveChngBtn->draw();
	closeBtn->draw();	

	for(auto param: params)
		param->draw();
}