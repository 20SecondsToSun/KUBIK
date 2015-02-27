#pragma once

#include "IScreen.h"
#include "ScreenSaverSettings.h"
#include "IResourceScreenSaver.h"
#include "VideoScreenSaver.h"
#include "ImageScreenSaver.h"
#include "gui/Sprite.h"

using namespace std;
using namespace ci;

namespace kubik
{
	class ScreenSaver: public IScreen, public Sprite
	{
	public:
		ScreenSaver(ISettingsRef config);

		void start();
		void stop();
		void init(ISettingsRef settings) override;
		void reset() override{};
		void draw();		

	protected:
		void setTextures();

	private:
		void mouseUp(EventGUIRef& event);
		connection mouseUpListener;

		IResourceScreenSaverRef screenSaverResource;
		ScreenSaverSettingsRef settings;
	};

	typedef shared_ptr<ScreenSaver> ScreenSaverRef;	
}