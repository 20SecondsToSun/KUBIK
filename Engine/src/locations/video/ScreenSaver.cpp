#include "ScreenSaver.h"

ScreenSaver::ScreenSaver()
{
	setTextures();
}

void ScreenSaver::setTextures()
{
	vector<ssFile> videos;
	string path = getAppPath().string() + "data\\screensaver\\";
	int videoIndex = -1;
	bigSizeError = false;

	for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
	{
		if (fs::is_regular_file(*it))
		{
			if (it->path().extension() == ".mov" ||
				it->path().extension() == ".jpg" || 
				it->path().extension() == ".jpeg"||
				it->path().extension() == ".png") 	
			{
				string filePath = path + it->path().filename().string();
				string ext = it->path().extension().string();

				if(fileSizeNotTooBig(filePath, it->path().extension().string()))
				{
					if(it->path().extension() == ".mov")
						videoIndex = videos.size();
				
					ssFile ss;
					ss.ext = ext;
					ss.path = filePath;
					videos.push_back(ss);
				}
				else
					bigSizeError = true;
			}
		}
	}

	_isEmpty = videos.empty();

	if(!_isEmpty)
	{	
		if(videoIndex == -1)
		{
			addToDictionary("image",	videos[0].path, resourceType::IMAGE, loadingType::FULL_PATH );
			mode = IMAGE_SS;
		}		
		else
		{
			addToDictionary("video",	videos[videoIndex].path, resourceType::VIDEO, loadingType::FULL_PATH );
			mode = VIDEO_SS;			
		}
	}
}

void ScreenSaver::init()
{
	if(mode == VIDEO_SS)
	{
		movie = designTexures["video"]->movie;
	}
	else if(mode == IMAGE_SS)
	{
		image = designTexures["image"]->tex;
	}
}

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

bool ScreenSaver::fileSizeNotTooBig(fs::path filePath, string ext)
{
	int filesizeInbytes  = (int)fileTools().filesize(filePath.string().c_str());
	int sizeLimit = MAX_IMAGE_FILE_SIZE;

	if (ext == ".mov")	
		sizeLimit = MAX_VIDEO_FILE_SIZE;
	
	return  filesizeInbytes < sizeLimit;
}

bool ScreenSaver::isError()
{
	return bigSizeError;
}

ServiceMessage ScreenSaver::getMessage()
{
	ServiceMessage msg(103);
	return msg;
}

bool ScreenSaver::isEmpty()
{
	return _isEmpty;
}

void ScreenSaver::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&ScreenSaver::mouseUp, this);
}

void ScreenSaver::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}