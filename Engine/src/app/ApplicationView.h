#pragma once

#include "IScreen.h"
#include "IGame.h"
#include "ServicePopup.h"
#include "preloader/Preloader.h"
#include "gui/Sprite.h"

using namespace std;
using namespace ci;

namespace kubik
{
	class ApplicationView
	{
	public:

		ApplicationView()
		{
			preloader	 = PreloaderRef(new Preloader());	
			servicePopup = ServicePopupRef(new ServicePopup());// font memory leak
		}

		void startLocation(IScreenRef screen)
		{				
			location = screen;
		}

		void draw()
		{
			location->draw();

			for (auto layer : layers)
				layer->draw();
		}

		void clearLayers()
		{
			layers.clear();
		}

		void addLayer(SpriteRef layer)
		{
			layers.push_back(layer);
		}

		void removeLayer(SpriteRef layer)
		{
			layers.remove(layer);
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//				PRELOADER
		//
		////////////////////////////////////////////////////////////////////////////

		void showPreloader()
		{
			kubik::setScreenShot(Utils::drawGraphicsToFBO(getWindowSize(), [&](){ if(location) draw(); }));
			preloader->setBackground(getScreenShot());
			location = preloader;
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//					SERVICE POPUP
		//
		////////////////////////////////////////////////////////////////////////////

		void showServicePopup(KubikException exc)
		{
			servicePopup->setMessage(exc.what());
			startLocation(servicePopup);
		}

	private:	
		IScreenRef location;
		ServicePopupRef servicePopup;
		PreloaderRef preloader;	

		std::list<SpriteRef> layers;
	};
}