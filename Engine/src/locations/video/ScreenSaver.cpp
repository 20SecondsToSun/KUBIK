#include "ScreenSaver.h"

void ScreenSaver::play()
{
	movie.setLoop( true, false );
	movie.play();
}

void ScreenSaver::stop()
{
	movie.stop();	
}

void ScreenSaver::load()
{
	vector<fs::path> videos;
	fs::path basePath = getAppPath()/"data"/"video";

	for (fs::directory_iterator it(basePath); it != fs::directory_iterator(); ++it)
	{
		if (fs::is_regular_file(*it))
		{
			if (it->path().extension() == ".mov") 	
			{
				fs::path filePath = basePath / it->path().filename().string();

				if(fileSizeNotTooBig(filePath))
					videos.push_back(filePath);
			}
		}
	}

	if(!videos.empty())
	{
		loadingStatus = VIDEO_LOADING;
		loadindUpdateConnection = App::get()->getSignalUpdate().connect( bind( &ScreenSaver::update, this ));	
		loadingThread = boost::shared_ptr<boost::thread>(new boost::thread(&ScreenSaver::loadMovieFile, this,  videos[0] ));
	}
	else
	{
		// no screensaver
		errorHandler();
	}
}

void ScreenSaver::update()
{
	if(loadingStatus == VIDEO_LOADED)
	{
		loadingThread->join();
		loadindUpdateConnection.disconnect();
		completeHandler();
	}
	else if(loadingStatus == VIDEO_LOADING_ERROR)
	{
		loadingThread->join();
		loadindUpdateConnection.disconnect();
		errorHandler();
	}
}

void ScreenSaver::loadMovieFileThread()
{

}


void ScreenSaver::loadMovieFile( const fs::path &moviePath )
{	
	try 
	{
		movie = qtime::MovieGl( moviePath );
		loadingStatus = VIDEO_LOADED;
		console()<<"loaded"<<endl;
		//completeHandler();

		//console() << "Dimensions:" << movie.getWidth() << " x " << movie.getHeight() << std::endl;
		//console() << "Duration:  " << movie.getDuration() << " seconds" << std::endl;
		//console() << "Frames:    " << movie.getNumFrames() << std::endl;
		//console() << "Framerate: " << movie.getFramerate() << std::endl;
	}
	catch( ... ) 
	{
		console() << "Unable to load the movie." << std::endl;
		loadingStatus = VIDEO_LOADING_ERROR;
		//errorHandler();
		return;
	}

	/*try 
	{
	movie.setupMonoFft( 8 );
	}
	catch( qtime::QuickTimeExcFft & ) {
	console() << "Unable to setup FFT" << std::endl;
	}*/		
}

void ScreenSaver::draw()
{
	Texture texture;
	if(movie)
	{
		texture = movie.getTexture();
		if(texture)
			gl::draw(texture, getWindowBounds());
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
