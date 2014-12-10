#pragma once
#include "cinder/app/AppNative.h"
#include "IScreen.h"
#include "IGame.h"

using namespace std;
using namespace ci;

namespace kubik
{
	class ApplicationView
	{
	public:

		void startLocation(shared_ptr<IScreen> screen)
		{	
			location = screen;
		}

		void startGameLocation(shared_ptr<IGame> screen)
		{	
			location = static_pointer_cast<IScreen>(screen);
		}

		void draw()
		{
			location->draw();
		}

	private:	
		shared_ptr<IScreen> location;	
	};
}