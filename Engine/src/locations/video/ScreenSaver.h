#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "IDrawable.h"
#include "FileTools.h"
#include "ServiceMessage.h"

#include <boost/thread.hpp>

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace ci::signals;

class ScreenSaver: public IDrawable
{
public:

	ScreenSaver();

	struct ssFile 
	{
		string path;
		string ext;
	};

	enum
	{
		IMAGE_SS,
		VIDEO_SS
	};

	void play();
	void stop();
	void init();	
	void draw();

	void addMouseUpListener();
	void removeMouseUpListener();

	void setTextures();

	signal<void(void)> closeVideoSignal;

	bool isEmpty();
	bool isError();
	ServiceMessage getMessage();

private:

	static const int MAX_VIDEO_FILE_SIZE = 50000000;	
	static const int MAX_IMAGE_FILE_SIZE = 10000000;	
	
	void mouseUp(MouseEvent &event);

	connection mouseUpListener;
	qtime::MovieGl movie;
	Surface	image;
	int mode;

	bool fileSizeNotTooBig(fs::path filePath, string ext);	
	bool _isEmpty, bigSizeError;
};