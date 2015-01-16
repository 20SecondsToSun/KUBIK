#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "FileTools.h"
#include "KubikException.h"
#include "IResourceScreenSaver.h"
#include "VideoScreenSaver.h"
#include "ImageScreenSaver.h"
#include "FileTools.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class ScreenSaverSettings:public ISettings
	{

	public:	
		struct ScreenSaverDataStruct
		{
			bool isActive;		
			string path;

			bool hasChanges(ScreenSaverDataStruct ss)
			{
				return (isActive != ss.isActive);
			}
		};

		ScreenSaverSettings(ApplicationModelRef model)
		{
			this->model = model;
			mainConfigPath = model->getScreenSaverConfigPath();				
		}

		void load() override
		{			
			JsonTree configJSON	= JsonTree(loadFile(mainConfigPath));
			data.path			= configJSON.getChild("path").getValue<string>();
			data.isActive		= configJSON.getChild("isActive").getValue<bool>();	

			findScreenSaver();
			setTextures();
		};

		void saveConfig()
		{
			console()<<"SAVE SCREENSAVER CONFIG"<<endl;

			fs::path basePath(mainConfigPath);

			JsonTree doc;		
			doc.addChild( JsonTree("path", data.path));
			doc.addChild( JsonTree("isActive", data.isActive));
			doc.write( writeFile(basePath), JsonTree::WriteOptions() );
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

		bool isActive()
		{
			return data.isActive;
		}

		bool isShow()
		{
			return isExist() && isActive();
		}

		void setTextures() override
		{
			if(mode == IMAGE_SS)
			{
				addToDictionary("image", createImageResource(path_ss));		
			}		
			else if(mode == VIDEO_SS)
			{
				addToDictionary("video", createVideoResource(path_ss));
			}	
		}

		shared_ptr<IResourceScreenSaver> getResource()
		{
			if(mode == VIDEO_SS)
			{
				screenSaverResource =  shared_ptr<VideoScreenSaver>(new VideoScreenSaver(videos["video"]->get()));	
			}
			else if(mode == IMAGE_SS)
			{
				screenSaverResource =  shared_ptr<ImageScreenSaver>(new ImageScreenSaver(textures["image"]->get()));	
			}

			return screenSaverResource;
		}

		ScreenSaverDataStruct getData()
		{
			return data;
		}	

		void setData(ScreenSaverDataStruct value)
		{
			data = value;
			saveConfig();
		}

	private:

		enum {IMAGE_SS, VIDEO_SS, NONE_SS};
		int mode;
		string path_ss;

		shared_ptr<IResourceScreenSaver> screenSaverResource;

		ScreenSaverDataStruct data;

		void findScreenSaver()
		{
			vector<string> content;	
			int videoIndex = -1;
			bool bigSizeError = false;			
			
			mode = NONE_SS;
			string path = getAppPath().string() + data.path;
			for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
			{
				if (fs::is_regular_file(*it))
				{
					string ext = it->path().extension().string();
					int ssType = getContentType(ext);
				
					if(ssType == NONE_SS)
						continue;

					string filePath = path + it->path().filename().string();

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

			if(fileTools().isVideoExtension(ext))
			{
				type = VIDEO_SS;
			}
			else if(fileTools().isImageExtension(ext))
			{
				type = IMAGE_SS;
			}

			return type;
		}		

		bool fileSizeNotTooBig(fs::path filePath, string ext)
		{
			int filesizeInbytes  = (int)fileTools().filesize(filePath.string().c_str());
			int sizeLimit;

			if (fileTools().isVideoExtension(ext))	
				sizeLimit = MAX_VIDEO_FILE_SIZE;
			else  if(fileTools().isImageExtension(ext))	
				sizeLimit = MAX_IMAGE_FILE_SIZE;

			return  filesizeInbytes < sizeLimit;
		}
	};

	typedef shared_ptr<ScreenSaverSettings> ScreenSaverSettingsRef;	
}