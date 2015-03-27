#include "ApplicationView.h"

using namespace std;
using namespace ci;
using namespace kubik;

ApplicationView::ApplicationView()
{
	preloader = PreloaderRef(new Preloader());
	servicePopup = ServicePopupRef(new ServicePopup());// font memory leak
}

void ApplicationView::startLocation(IScreenRef screen)
{
	location = screen;
}

void ApplicationView::draw()
{
	location->draw();

	for (auto layer : layers)
		layer->draw();
}

void ApplicationView::clearLayers()
{
	layers.clear();
}

void ApplicationView::addLayer(SpriteRef layer)
{
	layers.push_back(layer);
}

void ApplicationView::removeLayer(SpriteRef layer)
{
	layers.remove(layer);
}

////////////////////////////////////////////////////////////////////////////
//
//				PRELOADER
//
////////////////////////////////////////////////////////////////////////////

void ApplicationView::showPreloader()
{
	kubik::setScreenShot(Utils::drawGraphicsToFBO(getWindowSize(), [&](){ if (location) draw(); }));
	preloader->setBackground(getScreenShot());
	location = preloader;
}

////////////////////////////////////////////////////////////////////////////
//
//					SERVICE POPUP
//
////////////////////////////////////////////////////////////////////////////

void ApplicationView::showServicePopup(KubikException exc)
{
	servicePopup->setMessage(exc.what());
	startLocation(servicePopup);
}