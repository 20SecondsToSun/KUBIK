#include "ScreenSaver.h"

void ScreenSaver::play()
{
	if(mode == VIDEO_SS)
	{
		movie.setLoop( true, false );
		movie.play();
	}
}

void ScreenSaver::stop()
{
	if(mode == VIDEO_SS)
		movie.stop();	
}

void ScreenSaver::load()
{
	vector<ssFile> videos;
	fs::path basePath = getAppPath()/"data"/"screensaver";

	for (fs::directory_iterator it(basePath); it != fs::directory_iterator(); ++it)
	{
		if (fs::is_regular_file(*it))
		{
			if (it->path().extension() == ".mov" ||
				it->path().extension() == ".jpg" || 
				it->path().extension() == ".jpeg"||
				it->path().extension() == ".png") 	
			{
				fs::path filePath = basePath / it->path().filename().string();
				string ext = it->path().extension().string();

				if(fileSizeNotTooBig(filePath))
				{
					ssFile ss;
					ss.ext = ext;
					ss.path = filePath;
					videos.push_back(ss);
				}
			}
		}
	}

	if(!videos.empty())
	{
		loadingStatus = SCREEN_SAVER_LOADING;
		loadindUpdateConnection = App::get()->getSignalUpdate().connect(bind(&ScreenSaver::update, this));

		if (videos[0].ext == ".mov")
		{
			loadingThread = boost::shared_ptr<boost::thread>(new boost::thread(&ScreenSaver::loadMovieFile, this,  videos[0].path));
		}
		else
		{
			loadingThread = boost::shared_ptr<boost::thread>(new boost::thread(&ScreenSaver::loadImageFile, this,  videos[0].path));
		}
	}
	else
	{
		errorHandler();
	}
}

void ScreenSaver::update()
{
	if(loadingStatus == SCREEN_SAVER_LOADED)
	{
		loadingThread->join();
		loadindUpdateConnection.disconnect();
		completeHandler();
	}
	else if(loadingStatus == SCREEN_SAVER_LOADING_ERROR)
	{
		loadingThread->join();
		loadindUpdateConnection.disconnect();
		errorHandler();
	}
}

void ScreenSaver::loadImageFile( const fs::path &imagePath )
{
	try 
	{
		console()<<"try image loaded  "<<imagePath<<endl;
		image = ci::Surface(loadImage( ci::loadFile( imagePath ) )); 
		loadingStatus = SCREEN_SAVER_LOADED;
		mode = IMAGE_SS;
		console()<<"screen saver image loaded"<<endl;
	}
	catch( ... ) 
	{
		console() << "Unable to load the image." << std::endl;
		loadingStatus = SCREEN_SAVER_LOADING_ERROR;
	}
}

void ScreenSaver::loadMovieFile( const fs::path &moviePath )
{	
	try 
	{
		movie = qtime::MovieGl( moviePath);
		loadingStatus = SCREEN_SAVER_LOADED;
		mode = VIDEO_SS;
		console()<<"screen saver video loaded"<<endl;
		//completeHandler();

		//console() << "Dimensions:" << movie.getWidth() << " x " << movie.getHeight() << std::endl;
		//console() << "Duration:  " << movie.getDuration() << " seconds" << std::endl;
		//console() << "Frames:    " << movie.getNumFrames() << std::endl;
		//console() << "Framerate: " << movie.getFramerate() << std::endl;
	}
	catch( ... ) 
	{
		console() << "Unable to load the movie." << std::endl;
		loadingStatus = SCREEN_SAVER_LOADING_ERROR;
	}
}

void ScreenSaver::draw()
{
	Texture texture;
	if(mode == VIDEO_SS && movie)
	{
		texture = movie.getTexture();
		if(texture)
			gl::draw(texture, getWindowBounds());
	}
	else if(mode == IMAGE_SS)
	{
		gl::draw(image);
	}
}

void ScreenSaver::mouseUp(MouseEvent &event)
{
	closeVideoSignal();
}

bool ScreenSaver::fileSizeNotTooBig(fs::path filePath)
{
	return ((int)filesize(filePath.string().c_str()) < MAX_VIDEO_FILE_SIZE);
}

std::ifstream::pos_type ScreenSaver::filesize(const char* filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg(); 
}

void ScreenSaver::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&ScreenSaver::mouseUp, this);
}

void ScreenSaver::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}

void ScreenSaver::addCompleteListener(const std::function<void(void)>& handler)
{
	completeHandler = handler;
}

void ScreenSaver::addErrorListener(const std::function<void(void)>& handler)
{
	errorHandler = handler;
}

void ScreenSaver::removeCompleteListener()
{
	//completeHandler = NULL;
}

void ScreenSaver::removeErrorListener()
{
	//errorHandler = NULL;
}