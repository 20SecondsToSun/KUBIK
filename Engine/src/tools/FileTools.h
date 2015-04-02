#pragma once
#include <shellapi.h>
#include "cinder/Url.h"
#pragma comment(lib, "shell32")

using namespace ci;

namespace kubik
{
	static const int MAX_VIDEO_FILE_SIZE = 50000000;	
	static const int MAX_IMAGE_FILE_SIZE = 10000000;

	static const string IMAGE_SUPPORT_EXTENSIONS[3] = {".jpeg", ".jpg", ".png"};
	static const string VIDEO_SUPPORT_EXTENSIONS[1] = {".mov"};

	class FileTools
	{
	public:

		static FileTools& getInstance() { 
			static FileTools strt; 
			return strt; 
		};

		std::ifstream::pos_type filesize(const char* filename)
		{
			std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
			return in.tellg(); 
		}

		bool isVideoExtension(const std::string& ext)
		{
			for (auto it: VIDEO_SUPPORT_EXTENSIONS)
			{
				if (ext == it)
					return true;
			}

			return false;
		}

		bool isImageExtension(const std::string& ext)
		{
			for (auto it: IMAGE_SUPPORT_EXTENSIONS)
			{
				if (ext == it)
					return true;
			}

			return false;	
		}	

		void openSystemDirectory(const std::string& path )
		{
			std::wstring stemp = std::wstring(path.begin(), path.end());
			ShellExecute(NULL, L"open", stemp.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
		}

		void openURL(const std::string& path)
		{
			std::wstring stemp = std::wstring(path.begin(), path.end());
			ShellExecute(0, 0, stemp.c_str(), 0, 0, SW_SHOW);
		}		

		std::vector<std::string> getAllJpegPaths(const std::string& path)
		{
			std::vector<std::string> content;
			for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
			{
				if (fs::is_regular_file(*it))
				{
					string ext = it->path().extension().string();					

					if (!isImageExtension(ext))
						continue;

					string filePath = path + it->path().filename().string();
					content.push_back(filePath);
				}
			}
			return content;
		}		
	};

	// helper function(s) for easier access 
	inline FileTools& fileTools() {return FileTools::getInstance();};
}