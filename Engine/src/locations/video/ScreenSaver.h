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

		ScreenSaver(ScreenSaverSettings *settings);

		enum {IMAGE_SS,	VIDEO_SS, NONE_SS};		

		void start();
		void stop();
		void init();	
		void draw();

		void addMouseUpListener();
		void removeMouseUpListener();	

	protected:
		void setTextures();

	private:	

		void mouseUp(MouseEvent &event);
		connection mouseUpListener;		

		IResourceScreenSaver* screenSaverResource;
		ScreenSaverSettings* settings;

		int mode;
		string path_ss;
	};
}