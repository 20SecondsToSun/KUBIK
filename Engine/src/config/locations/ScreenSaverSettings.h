#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"
#include "ApplicationModel.h"
#include "FileTools.h"
#include "KubikException.h"
#include "IResourceScreenSaver.h"
#include "VideoScreenSaver.h"
#include "ImageScreenSaver.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	static const int MAX_VIDEO_FILE_SIZE = 50000000;	
	static const int MAX_IMAGE_FILE_SIZE = 10000000;

	static const string IMAGE_SUPPORT_EXTENSIONS[3] = {".jpeg", ".jpg", ".png"};
	static const string VIDEO_SUPPORT_EXTENSIONS[1] = {".mov"};

	class ScreenSaverSettings:public ISettings
	{
	public:	

		ScreenSaverSettings(ApplicationModel *model)
		{
			console()<<"findScreenSaver   "<<endl;
			this->model = model;
			findScreenSaver();
			setTextures();
		}

		int getScreenSaverMode()
		{
			return mode;
		}

		string getScreenSaverPath()
		{
			return path_ss;
		}

		bool isExist()
		{
			return mode != NONE_SS;
		}

		void setTextures()
		{
			if(mode == IMAGE_SS)
			{
				addToDictionary("image", path_ss);		
			}		
			else if(mode == VIDEO_SS)
			{
				addToDictionary("video", path_ss, resourceType::VIDEO, loadingType::FULL_PATH);
			}	
		}

		IResourceScreenSaver* getResource()
		{
			if(mode == VIDEO_SS)
			{
				screenSaverResource =  new VideoScreenSaver(designTexures["video"]->movie);	
			}
			else if(mode == IMAGE_SS)
			{
				screenSaverResource =  new ImageScreenSaver(designTexures["image"]->tex);	
			}

			return screenSaverResource;
		}

	private:

		enum {IMAGE_SS,	VIDEO_SS, NONE_SS};		
		int mode;
		string path_ss;

		ApplicationModel *model;

		IResourceScreenSaver* screenSaverResource;

		void findScreenSaver()
		{
			vector<string> content;	
			int videoIndex = -1;
			bool bigSizeError = false;			
			string PATH = model->getScreenSaverPath();
			mode = NONE_SS;

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
			
			if(content.empty() == false)
			{
				if(videoIndex == -1)
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

		int getContentType(string ext)
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

		bool isVideoExtension(string ext)
		{
			for (auto it: VIDEO_SUPPORT_EXTENSIONS)
			{
				if (ext == it)
					return true;
			}

			return false;
		}

		bool isImageExtension(string ext)
		{
			for (auto it: IMAGE_SUPPORT_EXTENSIONS)
			{
				if (ext == it)
					return true;
			}

			return false;	
		}		

		bool fileSizeNotTooBig(fs::path filePath, string ext)
		{
			int filesizeInbytes  = (int)fileTools().filesize(filePath.string().c_str());
			int sizeLimit;

			if (isVideoExtension(ext))	
				sizeLimit = MAX_VIDEO_FILE_SIZE;
			else  if(isImageExtension(ext))	
				sizeLimit = MAX_IMAGE_FILE_SIZE;

			return  filesizeInbytes < sizeLimit;
		}
	};
}