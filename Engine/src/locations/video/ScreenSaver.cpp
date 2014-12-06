#include "ScreenSaver.h"

using namespace kubik;

void ScreenSaver::setTextures()
{
	vector<string> content;	
	int videoIndex = -1;
	bool bigSizeError = false;

	string PATH = getScreenSaverPath();

	for (fs::directory_iterator it(PATH); it != fs::directory_iterator(); ++it)
	{
		if (fs::is_regular_file(*it))
		{
			string ext = it->path().extension().string();
			int ssType = getContentType(ext);

			if(ssType == NONE_SS)
				continue;

			string filePath = PATH + it->path().filename().string();				

			if(fileSizeNotTooBig(filePath, ext))
			{
				if(ssType == VIDEO_SS)				
					videoIndex = content.size();				
			
				content.push_back(filePath);
			}
			else
			{
				bigSizeError = true;
			}
		}
	}

	_isExist = content.empty();

	if(!_isExist)
	{	
		if(videoIndex == -1)
		{
			addToDictionary("image", content[0], resourceType::IMAGE, loadingType::FULL_PATH);
			mode = IMAGE_SS;
		}		
		else
		{
			addToDictionary("video", content[videoIndex], resourceType::VIDEO, loadingType::FULL_PATH);
			mode = VIDEO_SS;			
		}
	}
	else if (bigSizeError)
	{
		//throw

		//ServiceMessage msg(103);
		//return msg;
	}
}

int ScreenSaver::getContentType(string ext)
{
	int type = NONE_SS;

	if(isVideoExtension(ext))
	{
		type = VIDEO_SS;
	}
	else if(isImageExtension(ext))
	{
		type = IMAGE_SS;
	}

	return type;
}

bool ScreenSaver::fileSizeNotTooBig(fs::path filePath, string ext)
{
	int filesizeInbytes  = (int)fileTools().filesize(filePath.string().c_str());
	int sizeLimit;

	if (isVideoExtension(ext))	
		sizeLimit = MAX_VIDEO_FILE_SIZE;
	else  if(isImageExtension(ext))	
		sizeLimit = MAX_IMAGE_FILE_SIZE;
	
	return  filesizeInbytes < sizeLimit;
}

void ScreenSaver::init()
{
	if(mode == VIDEO_SS)
	{
		screenSaverResource =  new VideoScreenSaver(designTexures["video"]->movie);	
	}
	else if(mode == IMAGE_SS)
	{
		screenSaverResource =  new ImageScreenSaver(designTexures["image"]->tex);	
	}
}

void ScreenSaver::start()
{
	screenSaverResource->start();
}

void ScreenSaver::stop()
{	
	screenSaverResource->stop();
}

void ScreenSaver::draw()
{
	screenSaverResource->draw();
}

void ScreenSaver::mouseUp(MouseEvent &event)
{
	closeLocationSignal();
}

bool ScreenSaver::isVideoExtension(string ext)
{
	for (auto it: VIDEO_SUPPORT_EXTENSIONS)
	{
		if (ext == it)
			return true;
	}

	return false;
}

bool ScreenSaver::isImageExtension(string ext)
{
	for (auto it: IMAGE_SUPPORT_EXTENSIONS)
	{
		if (ext == it)
			return true;
	}

	return false;	
}

string ScreenSaver::getScreenSaverPath()
{
	return getAppPath().string() + SCREEN_SAVER_PATH;
}

bool ScreenSaver::isExist()
{
	return _isExist;
}

void ScreenSaver::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&ScreenSaver::mouseUp, this);
}

void ScreenSaver::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}