#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "IDrawable.h"
#include <boost/thread.hpp>

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace ci::signals;

class ScreenSaver: public IDrawable
{
public:

	void init()
	{

	}

	void play();
	void stop();

	void load();
	void draw();

	void addMouseUpListener();
	void removeMouseUpListener();

	signal<void(void)> closeVideoSignal, errorVideoSignal;

	void addCompleteListener(const std::function<void(void)>& handler);
	void addErrorListener(const std::function<void(void)>& handler);

	void removeCompleteListener();
	void removeErrorListener();

private:

	enum
	{
		VIDEO_LOADING,
		VIDEO_LOADED,
		VIDEO_LOADING_ERROR
	}loadingStates;

	void loadMovieFile(const fs::path &moviePath);
	void mouseUp(MouseEvent &event);

	connection mouseUpListener, loadindUpdateConnection;
	qtime::MovieGl movie;

	bool fileSizeNotTooBig(fs::path filePath);
	std::ifstream::pos_type filesize(const char* filename);

	std::function<void()> completeHandler;
	std::function<void()> errorHandler;

	boost::shared_ptr<boost::thread>	loadingThread;
	void loadMovieFileThread();

	void update();
	int loadingStatus;

	static const int MAX_VIDEO_FILE_SIZE = 20000000;
};