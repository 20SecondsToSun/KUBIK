#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"
#include "ApplicationModel.h"
#include "FileTools.h"
#include "KubikException.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	static const int MAX_VIDEO_FILE_SIZE = 50000000;	
	static const int MAX_IMAGE_FILE_SIZE = 10000000;

	static const string IMAGE_SUPPORT_EXTENSIONS[3] = {".jpeg", ".jpg", ".png"};
	static const string VIDEO_SUPPORT_EXTENSIONS[1] = {".mov"};

	static const string SCREEN_SAVER_PATH = "data\\screensaver\\";

	class ScreenSaverSettings:public ISettings
	{
	public:	

		ScreenSaverSettings(ApplicationModel *model)
		{
			findScreenSaver();
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
			return _isExist;
		}

	private:

		bool _isExist;
		enum {IMAGE_SS,	VIDEO_SS, NONE_SS};		
		int mode;
		string path_ss;

		void findScreenSaver()
		{
			vector<string> content;	
			int videoIndex = -1;
			bool bigSizeError = false;

			string PATH = getAppPath().string() + SCREEN_SAVER_PATH;

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

			_isExist = !content.empty();

			if(_isExist)
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