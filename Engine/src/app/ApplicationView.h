#pragma once

#include "IScreen.h"
#include "IGame.h"
#include "ServicePopup.h"
#include "Preloader.h"

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

		void startLocation(shared_ptr<IScreen> screen)
		{	
			location = screen;
		}

		void draw()
		{
			location->draw();
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//				PRELOADER
		//
		////////////////////////////////////////////////////////////////////////////

		void showPreloader()
		{
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
		PreloaderRef	 preloader;	
	};
}