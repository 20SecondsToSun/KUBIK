#include "TuneUpScreen.h"
using namespace kubik;

TuneUpScreen::TuneUpScreen(shared_ptr<TuneUpSettings>		config,
						   shared_ptr<ScreenSaverSettings> screenSaverSettings,
						   shared_ptr<MenuSettings>        menuConfig,
						   shared_ptr<GameSettings>		   gameSettings)
{	
	this->menuSettings		= menuConfig;
	this->gameSettings		= gameSettings;
	init(config);
}

TuneUpScreen::~TuneUpScreen()
{	
	console()<<"~~~~~~~~~~~~~~~ TuneUpScreen destructor ~~~~~~~~~~~~~~~"<<endl;
	//menuParams->clear();
	

	mouseUpListener.disconnect();
	closeBtnListener.disconnect();	
	appSettingsChgListener.disconnect();	
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

	font		 =  tuneUpSettings->getFonts()["helvetica90"]->get();
	Font fontBtn =  tuneUpSettings->getFonts()["helvetica20"]->get();

	saveChngBtn = shared_ptr<ButtonText>(new ButtonText(Rectf(800.0f, 450.0f, 950.0f, 550.0f), "Ñîõðàíèòü", fontBtn));	
	appSettingsChgListener = saveChngBtn->mouseUpSignal.connect(bind(&TuneUpScreen::appSettingsChgHandler, this, std::placeholders::_1));

	Texture closeImg = tuneUpSettings->getTextures()["closeImg"]->get();
	closeBtn = shared_ptr<Button>(new Button(closeImg, Vec2f(getWindowWidth() - 100.0f, 100.0f)));		
	closeBtnListener = closeBtn->mouseUpSignal.connect(bind(&TuneUpScreen::closeLocationHandler, this, std::placeholders::_1));

	
	createFuncesParams();
	createMenuParams();	
	createPhotoboothParams();
	
	createGamesParams();
}

void TuneUpScreen::createPhotoboothParams()
{

	photoBoothParams = params::InterfaceGl::create(getWindow(), "Photobooth parameters", toPixels( Vec2i( 300, 400)));
	photoBoothParams->addParam( "seconds", &photoboothData.seconds).min(3).max(5).step(1);
	photoBoothParams->addParam( "secondsBetweenShots", &photoboothData.secondsBetweenShots ).min(1).max( 3).step(1);
	photoBoothParams->addParam( "photoNum", &photoboothData.photoNum).min(1).max( 3 ).step(1);
	photoBoothParams->addSeparator();
	photoBoothParams->addParam( "Custom design",	 &photoboothData.isCustomDesign);
	photoBoothParams->addParam( "Template id",	 &photoboothData.templateId).min(1).max(2).step(1);
	photoBoothParams->addSeparator();
	photoBoothParams->addParam( "Facebook Sharing", &photoboothData.isFacebook);
	photoBoothParams->addParam( "Vkontakte Sharing", &photoboothData.isVkotakte);
	photoBoothParams->addParam( "Twitter Sharing",  &photoboothData.isTwitter);
	photoBoothParams->addParam( "Printer",		&photoboothData.isPrint);
	photoBoothParams->addParam( "Email",		&photoboothData.isEmail);
	photoBoothParams->addParam( "QrCode",		&photoboothData.isQrCode);
	photoBoothParams->hide();

	params.push_back(photoBoothParams);
}

void TuneUpScreen::createFuncesParams()
{

}

void TuneUpScreen::createMenuParams()
{
	menuData = menuSettings->getData();	
	menuParams = params::InterfaceGl::create(getWindow(), "Menu parameters", toPixels( Vec2i( 300, 200)));	
	menuParams->setPosition(Vec2i(350, 20));
	menuParams->addParam( "Custom design",	 &menuData.isCustomDesign);
	menuParams->addParam( "Template id",	 &menuData.templateId).min(1).max(2).step(1);
	menuParams->addSeparator();	
	menuParams->hide();

	params.push_back(menuParams);
}

void TuneUpScreen::createGamesParams()
{
	initialGamesData = gameSettings->getData();

	gamesParams = params::InterfaceGl::create(getWindow(), "Games parameters", toPixels( Vec2i( 300, 200)));
	gamesParams->setPosition(Vec2i(690, 20));

	for (auto game : initialGamesData.games)	
		if( game.isPurchased)		
			gamesParams->addParam( game.name, &game.isOn);		
	
	gamesParams->addSeparator();	
	gamesParams->addParam( "Default Game ID",	 &initialGamesData.defaultGameID).min(1).max(2).step(1);
	gamesParams->hide();

	params.push_back(gamesParams);
}

void TuneUpScreen::startUpParams()
{
	changes.clear();

	shared_ptr<PhotoboothSettings> phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(gameId::PHOTOBOOTH));
	photoboothData = phbthSettings->getData();
	initialPhotoboothData = phbthSettings->getData();

	menuData = menuSettings->getData();
	initialMenuData = menuSettings->getData();

	gamesData = gameSettings->getData();
	initialGamesData = gameSettings->getData();
}

void TuneUpScreen::closeLocationHandler(Button& button )
{	
	closeLocationSignal();
}

void TuneUpScreen::appSettingsChgHandler(ButtonText& button )
{
	checkPhotoBoothParamsForChanges();
	checkMenuParamsForChanges();
	checkGamesParamsForChanges();
	appSettingsChangedSignal(changes);
}

void TuneUpScreen::checkPhotoBoothParamsForChanges()
{
	shared_ptr<PhotoboothSettings> phbthSettings = static_pointer_cast<PhotoboothSettings>(gameSettings->get(gameId::PHOTOBOOTH));
	
	Changes chng;
	chng.texReload = false;
	chng.id = gameId::PHOTOBOOTH;

	if (initialPhotoboothData.isCustomDesign != photoboothData.isCustomDesign ||
		initialPhotoboothData.templateId != photoboothData.templateId)
	{
		chng.texReload = true;
		changes.push_back(chng);
		phbthSettings->setData(photoboothData);	
		phbthSettings->setDesignPath();		
		return;
	}

	if (initialPhotoboothData.isEmail != photoboothData.isEmail ||
		initialPhotoboothData.isFacebook != photoboothData.isFacebook ||
		initialPhotoboothData.isPrint != photoboothData.isPrint ||
		initialPhotoboothData.isQrCode != photoboothData.isQrCode ||
		initialPhotoboothData.isSticker != photoboothData.isSticker ||
		initialPhotoboothData.isTwitter != photoboothData.isTwitter ||
		initialPhotoboothData.isVkotakte != photoboothData.isVkotakte ||
		initialPhotoboothData.photoNum != photoboothData.photoNum ||
		initialPhotoboothData.seconds != photoboothData.seconds ||
		initialPhotoboothData.secondsBetweenShots != photoboothData.secondsBetweenShots ||
		initialPhotoboothData.stickerID != photoboothData.stickerID)
	{
		changes.push_back(chng);
		phbthSettings->setData(photoboothData);	
	}
}

void TuneUpScreen::checkMenuParamsForChanges()
{
	Changes chng;
	chng.texReload = false;
	chng.id = LocationID::MENU;

	if (initialMenuData.isCustomDesign != menuData.isCustomDesign ||
		initialMenuData.templateId != menuData.templateId)
	{
		chng.texReload = true;
		changes.push_back(chng);
		menuSettings->setData(menuData);	
		menuSettings->setDesignPath();		
		return;
	}
}

void TuneUpScreen::checkFuncesParamsForChanges()
{

}

void TuneUpScreen::checkGamesParamsForChanges()
{
	Changes chng;
	chng.texReload = false;
	chng.id = LocationID::MENU;
}

void TuneUpScreen::draw()
{
	gl::color(Color(1,0,0));
	gl::drawSolidRect(getWindowBounds());
	gl::color(Color::white());
	textTools().textFieldDraw("ÍÀÑÒÐÎÉÊÈ", &font, Vec2f(100, 100), Color::white());

	saveChngBtn->draw();
	closeBtn->draw();	

	for(auto param:params)
		param->draw();
}