#include "InstakubLocation.h"

using namespace kubik;
using namespace ci;
using namespace kubik::games::instakub;
using namespace instagram;

bool InstakubLocation::init = false;

InstagramClientRef InstakubLocation::instClient;
InstagramViewerRef InstakubLocation::instaViewer;
InstaPopupRef InstakubLocation::instaPopup;

gl::Texture InstakubLocation::bg;

InstakubLocation::InstakubLocation(InstakubSettingsRef settings, const Vec2f& position)
	:settings(settings) , yPosition(0.0f)
{
	this->position = position;

	if (init)
		return;	

	console() << "================================= CREATE ISTAGRAM VIEW =====================================" << endl;
	string clientID = "6ac8af15a5d341e9802c8d1a26899ae3";
	instClient  = InstagramClientRef(new InstagramClient(clientID));

	instaViewer = InstagramViewerRef(new InstagramViewer(instClient,
		settings->getTexture("preloaderMain"),
		settings->getTexture("preloaderMini"),
		settings->getTexture("noMaterials"),
		settings->getTexture("allLoaded")));	

	instaPopup = InstaPopupRef(new InstaPopup(instClient,
		settings->getTexture("closeInstaPopup"), 
		settings->getTexture("printInstaPopup"), 
		settings->getCurrentTemplate()));

	init = true;
}

void InstakubLocation::initPosition()
{
	instaViewer->setPosition(position.x, position.y);
}

void InstakubLocation::clear()
{
	instaViewer->clear();
}

void InstakubLocation::start()
{	
	instaViewer->showPreloader();
}

void InstakubLocation::stop()
{
	instaViewer->touchedEvent.disconnect_all_slots();
	instaViewer->reloadAllMedia.disconnect_all_slots();
	instaViewer->loadNextMedia.disconnect_all_slots();

	instClient->startLoadEvent.disconnect_all_slots();
	instClient->noMoreEvent.disconnect_all_slots();
	instClient->synchEvent.disconnect_all_slots();

	instaViewer->disconnect();
	instaViewer->clear();
	disconnectPopup();
}

void InstakubLocation::draw()
{
	fillBg();
	//drawTitle();

	gl::pushMatrices();
	gl::color(Color::white());
	instaViewer->draw();
	gl::popMatrices();
};

void InstakubLocation::reset()
{
	bg = settings->getTexture("bg");	
}

void InstakubLocation::initOverMask()
{
	overMask = Utils::drawGraphicsToFBO(Vec2f(getWindowWidth(), position.y), [&](){ gl::draw(bg); });
}

void InstakubLocation::fillBg()
{
	gl::draw(bg);
};

void InstakubLocation::hashtagPhotosload(const string& hashtag)
{
	connect_once(instaViewer->touchedEvent, bind(&InstakubLocation::openPopupHandler, this));
	connect_once(instaViewer->reloadAllMedia, bind(&InstakubLocation::reloadHandler, this));
	connect_once(instaViewer->loadNextMedia, bind(&InstakubLocation::nextLoadHandler, this));
	instaViewer->connect();

	connect_once(instClient->synchEvent, bind(&InstakubLocation::loadingCompleteHandler, this));
	connect_once(instClient->startLoadEvent, bind(&InstakubLocation::startLoadHandler, this));
	connect_once(instClient->noMoreEvent, bind(&InstakubLocation::noMoreLoadsHandler, this));

	instClient->loadTagMedia(hashtag);	
}

void InstakubLocation::userPhotosload(const string& userName)
{
	connect_once(instaViewer->touchedEvent, bind(&InstakubLocation::openPopupHandler, this));
	connect_once(instaViewer->reloadAllMedia, bind(&InstakubLocation::reloadHandler, this));
	connect_once(instaViewer->loadNextMedia, bind(&InstakubLocation::nextLoadHandler, this));
	instaViewer->connect();

	connect_once(instClient->synchEvent, bind(&InstakubLocation::loadingCompleteHandler, this));
	connect_once(instClient->startLoadEvent, bind(&InstakubLocation::startLoadHandler, this));
	connect_once(instClient->noMoreEvent, bind(&InstakubLocation::noMoreLoadsHandler, this));

	instClient->loadUserMedia(userName);
}

void InstakubLocation::popularPhotosLoad()
{
	console() << "LOAD STRAGEDY:::::::::::::::: POPULAR_PHOTOS_LOAD :::: " << endl;

	connect_once(instaViewer->touchedEvent, bind(&InstakubLocation::openPopupHandler, this));
	instaViewer->connect();

	connect_once(instClient->synchEvent, bind(&InstakubLocation::loadingCompleteHandler, this));
	connect_once(instClient->startLoadEvent, bind(&InstakubLocation::startLoadHandler, this));
	connect_once(instClient->noMoreEvent, bind(&InstakubLocation::noMoreLoadsHandler, this));

	instClient->loadPopular();	
}

void InstakubLocation::loadingCompleteHandler()
{
	instaViewer->synchImages();
	callback(ENABLE_CONTROLS);
}

void InstakubLocation::startLoadHandler()
{
	callback(DISABLE_CONTROLS);
}

void InstakubLocation::noMoreLoadsHandler()
{
	instaViewer->showNoMoreImagesMsg();
	callback(ENABLE_CONTROLS);
}

void InstakubLocation::openPopupHandler()
{
	callback(HIDE_CONTROLS);
	instaViewer->disconnect();

	instaPopup->show(instaViewer->getImageGraphic());
	instaPopup->connectEventHandler(&InstakubLocation::closePopupHandler, this, InstaPopup::CLOSE_POPUP);
	instaPopup->connectEventHandler(&InstakubLocation::printPopupHandler, this, InstaPopup::PRINT);
	instaPopup->activateListeners();
}

void InstakubLocation::nextLoadHandler()
{
	instClient->loadNextMedia();
	instaViewer->showMiniPreloader();
}

void InstakubLocation::reloadHandler()
{
	reload();
}

void InstakubLocation::closePopupHandler()
{
	disconnectPopup();
	instaViewer->connect();
	callback(SHOW_CONTROLS);
}

void InstakubLocation::disconnectPopup()
{
	instaPopup->disconnectEventHandler(InstaPopup::CLOSE_POPUP);
	instaPopup->disconnectEventHandler(InstaPopup::PRINT);
	instaPopup->unActivateListeners();
	instaPopup->hide();
}

void InstakubLocation::printPopupHandler()
{
	console() << "..............printing................" << endl;
}

void InstakubLocation::drawPopup()
{
	instaPopup->draw();
}

void InstakubLocation::reload()
{
	if (instaPopup->isOpen())
		closePopupHandler();
	
	clear();
	
	//start();
	instaViewer->showPreloader();	
	loadStrategity();
}

void InstakubLocation::loadStrategity()
{	
	switch (mode)
	{
	case HASHTAG_DEFAULT_PHOTOS_LOAD:
		hashTagDefaultPhotosLoad();
		break;

	case POPULAR_PHOTOS_LOAD:
		popularPhotosLoad();
		break;
	}
}

void InstakubLocation::hashTagDefaultPhotosLoad()
{
	string hashtag = settings->getHashtag();
	hashtagPhotosload(hashtag);

	console() << "LOAD STRAGEDY:::::::::::::::: HASHTAG_DEFAULT_PHOTOS_LOAD :::: " << hashtag << endl;
}