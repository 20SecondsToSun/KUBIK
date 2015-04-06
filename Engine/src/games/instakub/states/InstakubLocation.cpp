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

InstakubLocation::InstakubLocation(InstakubSettingsRef settings)
	:settings(settings) , yPosition(0.0f)
{
	if (init)
		return;	

	console() << "================================= CREATE ISTAGRAM VIEW =====================================" << endl;
	string clientID = "6ac8af15a5d341e9802c8d1a26899ae3";
	instClient = InstagramClientRef(new InstagramClient(clientID));
	instaViewer = InstagramViewerRef(new InstagramViewer(instClient));
	instaPopup = InstaPopupRef(new InstaPopup(instClient,
		settings->getTexture("closeInstaPopup"), 
		settings->getTexture("printInstaPopup"), 
		settings->getCurrentTemplate()));

	init = true;
}

void InstakubLocation::setPosition(float x, float y)
{
	instaViewer->setPosition(x, y);
}

void InstakubLocation::clear()
{
	instaViewer->clear();
}

void InstakubLocation::start()
{
	instaViewer->touchedEvent.connect(bind(&InstakubLocation::touchedHandler, this));
	instaViewer->connect();
}

void InstakubLocation::stop()
{
	instaViewer->touchedEvent.disconnect_all_slots();
	instaViewer->disconnect();
	instaViewer->clear();
	disconnectPopup();
}

void InstakubLocation::draw()
{
	fillBg();
	drawTitle();

	gl::pushMatrices();
	gl::color(Color::white());
	instaViewer->draw();
	gl::popMatrices();
};

void InstakubLocation::reset()
{
	bg = settings->getTexture("bg");	
}

void InstakubLocation::fillBg()
{
	gl::draw(bg);
};

void InstakubLocation::drawTitle()
{

};

void InstakubLocation::load()
{
	instClient->synchEvent.connect(bind(&InstakubLocation::synchHandler, this));
	instClient->startLoadEvent.connect(bind(&InstakubLocation::startLoadHandler, this));

	instClient->loadTagMedia("nature");
}

void InstakubLocation::synchHandler()
{
	instaViewer->synchImages();
	callback(ENABLE_CONTROLS);
}

void InstakubLocation::startLoadHandler()
{
	callback(DISABLE_CONTROLS);
}

void InstakubLocation::touchedHandler()
{
	callback(HIDE_CONTROLS);
	instaViewer->disconnect();

	instaPopup->show(instaViewer->getImageGraphic());
	instaPopup->connectEventHandler(&InstakubLocation::closePopupHandler, this, InstaPopup::CLOSE_POPUP);
	instaPopup->connectEventHandler(&InstakubLocation::printPopupHandler, this, InstaPopup::PRINT);
	instaPopup->activateListeners();
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