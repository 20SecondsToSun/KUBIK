#include "TuneUpScreen.h"
using namespace kubik;

TuneUpScreen::TuneUpScreen(TuneUpSettings* config, MenuSettings* menuConfig, GameSettings* gameSettings):IScreen()
{	
	settings			= config;
	this->menuConfig	= menuConfig;
	this->gameSettings	= gameSettings;
}

TuneUpScreen::~TuneUpScreen()
{	
	console()<<"~~~~~~~~~~~~~~~ TuneUpScreen destructor"<<endl;
	mouseUpListener.disconnect();
	closeBtnListener.disconnect();	
	appSettingsChgListener.disconnect();	
}

void TuneUpScreen::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&TuneUpScreen::mouseUp, this);	
	photoBoothParams->show();
	menuParams->show();
}

void TuneUpScreen::mouseUp(MouseEvent &event)
{
	closeBtn->mouseUpHandler(event.getPos());
	saveChngBtn->mouseUpHandler(event.getPos());	
}

void TuneUpScreen::removeMouseUpListener()
{
	mouseUpListener.disconnect();
	photoBoothParams->hide();
	menuParams->hide();
}

void TuneUpScreen::init(TuneUpSettings* settings)
{
	console()<<"set settings screen"<<endl;

	font		 =  settings->getTextures()["helvetica90"]->font;
	Font fontBtn =  settings->getTextures()["helvetica20"]->font;

	saveChngBtn = new ButtonText(Rectf(800.0f, 450.0f, 950.0f, 550.0f), "Ñîõðàíèòü", fontBtn);	
	appSettingsChgListener = saveChngBtn->mouseUpSignal.connect(bind(&TuneUpScreen::appSettingsChgHandler, this, std::placeholders::_1));

	Texture closeImg = settings->getTextures()["closeImg"]->tex;
	closeBtn = shared_ptr<Button>(new Button(closeImg, Vec2f(getWindowWidth() - 100, 100)));		
	closeBtnListener = closeBtn->mouseUpSignal.connect(bind(&TuneUpScreen::closeLocationHandler, this, std::placeholders::_1));

	createPhotoboothParams();
	createMenuParams();
}

void TuneUpScreen::createPhotoboothParams()
{
	photoBoothParams = params::InterfaceGl::create(getWindow(), "Photobooth parameters", toPixels( Vec2i( 300, 400)));
	photoBoothParams->addParam( "seconds", &phSet.seconds).min(3).max(5).step(1);
	photoBoothParams->addParam( "secondsBetweenShots", &phSet.secondsBetweenShots ).min(1).max( 3).step(1);
	photoBoothParams->addParam( "photoNum", &phSet.photoNum).min(1).max( 3 ).step(1);
	photoBoothParams->addParam( "Custom design",	 &phSet.isCustomDesign);
	photoBoothParams->addSeparator();
	photoBoothParams->addParam( "Facebook Sharing", &phSet.isFacebook);
	photoBoothParams->addParam( "Vkontakte Sharing", &phSet.isVkotakte);
	photoBoothParams->addParam( "Twitter Sharing",  &phSet.isTwitter);
	photoBoothParams->addParam( "Printer",		&phSet.isPrinter);
	photoBoothParams->addParam( "Email",		&phSet.isEmail);
	photoBoothParams->addParam( "QrCode",		&phSet.isQrCode);
	photoBoothParams->addSeparator();
	
	photoBoothParams->addSeparator();
	photoBoothParams->addButton( "SAVE!", std::bind( &TuneUpScreen::savePhtbtn, this ) );
	photoBoothParams->hide();
}

void TuneUpScreen::createMenuParams()
{
	menuParams = params::InterfaceGl::create(getWindow(), "Menu parameters", toPixels( Vec2i( 300, 400)));
	menuParams->setPosition(Vec2i(600, 20));
	menuParams->hide();
}

void TuneUpScreen::startUpParams()
{
	changes.clear();

	PhotoboothSettings* phbthSettings = gameSettings->getPhotoboothSettings();
	phSet.isFacebook			 = phbthSettings->getFacebook();
	phSet.isVkotakte			 = phbthSettings->getVontakte();
	phSet.seconds				 = phbthSettings->getSeconds();
	phSet.secondsBetweenShots	 = phbthSettings->getSecondsBetweenShots();
	phSet.photoNum				 = phbthSettings->getPhotoNum();
}

void TuneUpScreen::savePhtbtn()
{	
	changes.push_back(gameId::PHOTOBOOTH);
}

void TuneUpScreen::savePhotoboothParams()
{
	// TO DO CHECK FOR CHANGES
	PhotoboothSettings* phbthSettings = gameSettings->getPhotoboothSettings();
	phbthSettings->setFacebook(phSet.isFacebook);
	phbthSettings->setVontakte(phSet.isVkotakte);
	phbthSettings->setSeconds(phSet.seconds);
	phbthSettings->setSecondsBetweenShots(phSet.secondsBetweenShots);
	phbthSettings->setPhotoNum(phSet.photoNum);
	phbthSettings->saveConfig();
}

void TuneUpScreen::closeLocationHandler(Button& button )
{	
	closeLocationSignal();
}

void TuneUpScreen::appSettingsChgHandler(ButtonText& button )
{
	for (auto change: changes)
	{
		if(change == gameId::PHOTOBOOTH)
		{
			savePhotoboothParams();
		}
	}

	appSettingsChangedSignal(changes);
}

void TuneUpScreen::draw()
{
	gl::color(Color(1,0,0));
	gl::drawSolidRect(getWindowBounds());
	gl::color(Color::white());
	textTools().textFieldDraw("ÍÀÑÒÐÎÉÊÈ", &font, Vec2f(100, 100), Color::white());

	saveChngBtn->draw();
	closeBtn->draw();	
	photoBoothParams->draw();
	menuParams->draw();
}