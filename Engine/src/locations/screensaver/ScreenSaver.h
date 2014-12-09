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

		void start();
		void stop();
		void init(ScreenSaverSettings* settings);
		void draw();

		void addMouseUpListener();
		void removeMouseUpListener();	

	protected:
		void setTextures();

	private:	

		void mouseUp(MouseEvent &event);
		ci::signals::connection mouseUpListener;

		IResourceScreenSaver* screenSaverResource;
		ScreenSaverSettings* settings;
	};
}