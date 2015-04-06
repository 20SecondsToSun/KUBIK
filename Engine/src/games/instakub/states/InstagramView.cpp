#include "instakub/states/InstagramView.h"

using namespace kubik;
using namespace kubik::config;
using namespace kubik::games::instakub;

InstagramView::InstagramView(const gl::Texture& close, const gl::Texture& save, const gl::Texture& _template)
	:yPosition(0.0f)
{
	console() << "================================= CREATE ISTAGRAM VIEW =====================================" << endl;
	string clientID = "6ac8af15a5d341e9802c8d1a26899ae3";
	instClient = InstagramClientRef(new InstagramClient(clientID));
	instaViewer = InstagramViewerRef(new InstagramViewer(instClient));
	instaPopup = InstaPopupRef(new InstaPopup(instClient, close, save, _template));
}

void InstagramView::setPosition(float x, float y)
{
	instaViewer->setPosition(x, y);
}

void InstagramView::clear()
{
	instaViewer->clear();
}

void InstagramView::load()
{	
	instClient->synchEvent.connect(bind(&InstagramView::synchHandler, this));
	instClient->startLoadEvent.connect(bind(&InstagramView::startLoadHandler, this));

	instClient->loadTagMedia("nature");
}

void InstagramView::synchHandler()
{
	instaViewer->synchImages();
	callback(1);// ENABLE CONTROLS	
}

void InstagramView::startLoadHandler()
{
	callback(2);// DISABLE CONTROLS
}

void InstagramView::start()
{	
	instaViewer->touchedEvent.connect(bind(&InstagramView::touchedHandler, this));
	instaViewer->connect();
}

void InstagramView::stop()
{
	instaViewer->touchedEvent.disconnect_all_slots();
	instaViewer->disconnect();
	instaViewer->clear();
	disconnectPopup();
}

void InstagramView::touchedHandler()
{
	callback(4);// HIDE CONTROLS

	instaViewer->disconnect();

	instaPopup->show(instaViewer->getImageGraphic());
	instaPopup->connectEventHandler(&InstagramView::closePopupHandler, this, InstaPopup::CLOSE_POPUP);
	instaPopup->connectEventHandler(&InstagramView::printPopupHandler, this, InstaPopup::PRINT);
	instaPopup->activateListeners();
}

void InstagramView::closePopupHandler()
{
	disconnectPopup();
	instaViewer->connect();
	callback(3);// SHOW CONTROLS
}

void InstagramView::disconnectPopup()
{
	instaPopup->disconnectEventHandler(InstaPopup::CLOSE_POPUP);
	instaPopup->disconnectEventHandler(InstaPopup::PRINT);
	instaPopup->unActivateListeners();
	instaPopup->hide();
}

void InstagramView::printPopupHandler()
{
	console() << "..............printing................" << endl;
}

void InstagramView::draw()
{
	gl::pushMatrices();
	gl::color(Color::white());
	instaViewer->draw();	
	gl::popMatrices();	
}

void InstagramView::drawPopup()
{
	instaPopup->draw();
}