#include "ApplicationView.h"

using namespace std;
using namespace ci;
using namespace kubik;
using namespace kubik::config;

ApplicationView::ApplicationView()
{
	preloader	 = PreloaderRef(new Preloader());
	servicePopup = ServicePopupRef(new ServicePopup()); // font memory leak TODO
}

void ApplicationView::startLocation(IScreenRef screen)
{
	location = screen;
}

void ApplicationView::draw()
{
	location->draw();

	for (auto layer : layers)
	{
		layer->draw();
	}		
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
	kubik::setScreenShot(Utils::drawGraphicsToFBO(app::getWindowSize(), [&](){ if (location) draw(); }));

	if (settingsFactory().getMainPreloader() != nullptr)
	{
		preloader->set(settingsFactory().getMainPreloader());
	}
		
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