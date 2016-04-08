#pragma once
#include <shellapi.h>
#include "cinder/Url.h"
#pragma comment(lib, "shell32")

using namespace ci;

namespace kubik
{
	static const int MAX_VIDEO_FILE_SIZE = 50000000;	
	static const int MAX_IMAGE_FILE_SIZE = 10000000;

	static const std::string IMAGE_SUPPORT_EXTENSIONS[3] = {".jpeg", ".jpg", ".png"};
	static const std::string VIDEO_SUPPORT_EXTENSIONS[2] = { ".mov", ".mp4" };

	class FileTools
	{
		public:
			static FileTools& getInstance();
			std::ifstream::pos_type filesize(const char* filename);
			bool isVideoExtension(const std::string& ext);
			bool isImageExtension(const std::string& ext);
			void openSystemDirectory(const std::string& path);
			void openURL(const std::string& path);
			std::vector<std::string> getAllImagePaths(const std::string& path);
			std::string getVideoPath(const std::string& path);
	};

	inline FileTools& fileTools() {return FileTools::getInstance();};
}