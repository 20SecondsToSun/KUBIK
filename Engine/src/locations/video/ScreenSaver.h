#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "IScreen.h"
#include "FileTools.h"
#include "ServiceMessage.h"
#include "IResourceScreenSaver.h"
#include "VideoScreenSaver.h"
#include "ImageScreenSaver.h"

using namespace std;
using namespace ci;

namespace kubik
{
	static const int MAX_VIDEO_FILE_SIZE = 50000000;	
	static const int MAX_IMAGE_FILE_SIZE = 10000000;

	static const string IMAGE_SUPPORT_EXTENSIONS[3] = {".jpeg", ".jpg", ".png"};
	static const string VIDEO_SUPPORT_EXTENSIONS[1] = {".mov"};

	static const string SCREEN_SAVER_PATH = "data\\screensaver\\";

	class ScreenSaver: public IScreen
	{
	public:

		ScreenSaver():IScreen()
		{		
			setTextures();
		}

		enum {IMAGE_SS,	VIDEO_SS, NONE_SS};		

		void start();
		void stop();
		void init();	
		void draw();

		void addMouseUpListener();
		void removeMouseUpListener();		

		bool isExist();
		ServiceMessage getMessage();

	protected:
		void setTextures();

	private:	

		void mouseUp(MouseEvent &event);

		connection mouseUpListener;

		Surface	image;
		int mode;

		bool fileSizeNotTooBig(fs::path filePath, string ext);	
		bool _isExist;

		bool isVideoExtension(string ext);
		bool isImageExtension(string ext);

		string getScreenSaverPath();
		int getContentType(string ext);

		IResourceScreenSaver* screenSaverResource;
	};
}