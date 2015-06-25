#include "ScreenSaverSettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik::config;
using namespace kubik;

ScreenSaverSettings::ScreenSaverSettings(ApplicationModelRef model) 
	:ISettings(model),
	memento(false)
{
	mainConfigPath = model->getScreenSaverConfigPath();
}

void ScreenSaverSettings::load()
{
	try
	{
		JsonTree configJSON = JsonTree(loadFile(mainConfigPath));
		path = configJSON.getChild("path").getValue<string>();
		isActive = configJSON.getChild("isActive").getValue<bool>();
		
		findScreenSaver();
	}
	catch (...)
	{
		throw ExcConfigFileParsing();
	}

	setTextures();
};

void ScreenSaverSettings::saveConfig()
{	
	fs::path basePath(mainConfigPath);

	JsonTree doc;
	doc.addChild(JsonTree("path", path));
	doc.addChild(JsonTree("isActive", isActive));
	doc.write(writeFile(basePath), JsonTree::WriteOptions());
}

int ScreenSaverSettings::getScreenSaverMode() const
{
	return mode;
}

string ScreenSaverSettings::getScreenSaverPath() const
{
	return path_ss;
}

bool ScreenSaverSettings::getActive() const
{
	return isActive;
}

void ScreenSaverSettings::setActive(bool value)
{
	isActive = value;
}

string ScreenSaverSettings::getPath() const
{
	return getAppPath().string() + path;
}

bool ScreenSaverSettings::isExist() const
{
	return mode != NONE_SS;
}

bool ScreenSaverSettings::isShow() const
{
	return isExist() && isActive;
}

void ScreenSaverSettings::setTextures()
{
	if (mode == IMAGE_SS)
	{
		addToDictionary("image", createImageResource(path_ss));
	}
	else if (mode == VIDEO_SS)
	{
		addToDictionary("video", createVideoResource(path_ss));
	}
}

shared_ptr<IResourceScreenSaver> ScreenSaverSettings::getResource()
{
	if (mode == VIDEO_SS)	
		screenSaverResource = shared_ptr<VideoScreenSaver>(new VideoScreenSaver(videos["video"]->get()));
	else if (mode == IMAGE_SS)	
		screenSaverResource = shared_ptr<ImageScreenSaver>(new ImageScreenSaver(textures["image"]->get()));
	
	return screenSaverResource;
}

void ScreenSaverSettings::createMemento()
{
	isActiveMemento = isActive;
	memento = true;
};

void ScreenSaverSettings::writeConfig()
{
	if (memento)
	{
		fs::path basePath(mainConfigPath);
		JsonTree doc;
		doc.addChild(JsonTree("path", path));
		doc.addChild(JsonTree("isActive", isActive));
		
		doc.write(writeFile(basePath), JsonTree::WriteOptions());
		logger().log("ScreenSaverSettings CONFIG CHANGED!!!!");
	}
};

bool ScreenSaverSettings::settingsChanged()
{ 
	return isActiveMemento != isActive;
};

changeSetting::id ScreenSaverSettings::getChangeID() const 
{ 
	return changeSetting::id::SCREENSAVER; 
};

void ScreenSaverSettings::findScreenSaver()
{
	vector<string> content;
	int videoIndex = -1;
	bool bigSizeError = false;

	mode = NONE_SS;

	string _path = getAppPath().string() + path;

	for (fs::directory_iterator it(_path); it != fs::directory_iterator(); ++it)
	{
		if (fs::is_regular_file(*it))
		{
			string ext = it->path().extension().string();
			int ssType = getContentType(ext);

			if (ssType == NONE_SS)
				continue;

			string filePath = _path + it->path().filename().string();

			if (fileSizeNotTooBig(filePath, ext))
			{
				if (ssType == VIDEO_SS)
					videoIndex = content.size();

				content.push_back(filePath);
			}
			else
			{
				bigSizeError = true;
			}
		}
	}

	if (content.empty() == false)
	{
		if (videoIndex == -1)
		{
			path_ss = content[0];
			mode = IMAGE_SS;
		}
		else
		{
			path_ss = content[videoIndex];
			mode = VIDEO_SS;
		}
	}
	else if (bigSizeError)
		throw ExcBigFileSizeOfScreenSaver();
}

int ScreenSaverSettings::getContentType(const string& ext)
{
	int type = NONE_SS;

	if (fileTools().isVideoExtension(ext))
	{
		type = VIDEO_SS;
	}
	else if (fileTools().isImageExtension(ext))
	{
		type = IMAGE_SS;
	}

	return type;
}

bool ScreenSaverSettings::fileSizeNotTooBig(const fs::path& filePath, const string& ext)
{
	int filesizeInbytes = (int)fileTools().filesize(filePath.string().c_str());
	int sizeLimit = 100;

	if (fileTools().isVideoExtension(ext))
		sizeLimit = MAX_VIDEO_FILE_SIZE;
	else  if (fileTools().isImageExtension(ext))
		sizeLimit = MAX_IMAGE_FILE_SIZE;

	return  filesizeInbytes < sizeLimit;
}