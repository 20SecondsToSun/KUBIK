#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

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
		ScreenSaver(shared_ptr<ISettings> settings);
		void start();
		void stop();
		void init(shared_ptr<ISettings> settings) override;
		void reset(shared_ptr<ISettings> config) override{};
		void draw();

		void addMouseUpListener();
		void removeMouseUpListener();	

	protected:
		void setTextures();

	private:	

		void mouseUp(MouseEvent &event);
		ci::signals::connection mouseUpListener;

		shared_ptr<IResourceScreenSaver> screenSaverResource;
		shared_ptr<ScreenSaverSettings> settings;
	};
}