#pragma once

#include "IScreen.h"
#include "ScreenSaverSettings.h"
#include "IResourceScreenSaver.h"
#include "VideoScreenSaver.h"
#include "ImageScreenSaver.h"
#include "gui/Sprite.h"

namespace kubik
{
	class ScreenSaver: public IScreen, public Sprite
	{
	public:
		ScreenSaver(config::ISettingsRef config);

		void start();
		void stop();
		void init(config::ISettingsRef settings) override;
		void reset() override{};
		void draw();		

	protected:
		void setTextures();

	private:
		void mouseUp(EventGUIRef& event);
		ci::signals::connection mouseUpListener;

		IResourceScreenSaverRef screenSaverResource;
		config::ScreenSaverSettingsRef settings;
	};

	typedef std::shared_ptr<ScreenSaver> ScreenSaverRef;	
}