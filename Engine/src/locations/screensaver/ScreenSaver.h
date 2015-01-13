#pragma once

#include "IScreen.h"
#include "ScreenSaverSettings.h"
#include "IResourceScreenSaver.h"
#include "VideoScreenSaver.h"
#include "ImageScreenSaver.h"

using namespace std;
using namespace ci;

namespace kubik
{
	class ScreenSaver: public IScreen
	{
	public:
		ScreenSaver(ISettingsRef config);

		void start();
		void stop();

		void init(ISettingsRef settings) override;
		void reset() override{};

		void draw();

		void addMouseUpListener();
		void removeMouseUpListener();	

	protected:
		void setTextures();

	private:
		void mouseUp(MouseEvent &event);
		ci::signals::connection mouseUpListener;

		shared_ptr<IResourceScreenSaver> screenSaverResource;
		ScreenSaverSettingsRef settings;
	};

	typedef shared_ptr<ScreenSaver> ScreenSaverRef;	
}