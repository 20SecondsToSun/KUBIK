#include "FileTools.h"

using namespace ci;
using namespace kubik;
using namespace std;

FileTools& FileTools::getInstance()
{
	static FileTools strt;
	return strt;
}

std::ifstream::pos_type FileTools::filesize(const char* filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

bool FileTools::isVideoExtension(const std::string& ext)
{
	for (auto it : VIDEO_SUPPORT_EXTENSIONS)
	{
		if (ext == it)
		{
			return true;
		}
	}

	return false;
}

bool FileTools::isImageExtension(const std::string& ext)
{
	for (auto it : IMAGE_SUPPORT_EXTENSIONS)
	{
		if (ext == it)
		{
			return true;
		}
	}

	return false;
}

void FileTools::openSystemDirectory(const std::string& path)
{
	std::wstring stemp = std::wstring(path.begin(), path.end());
	ShellExecute(NULL, L"open", stemp.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
}

void FileTools::openURL(const std::string& path)
{
	std::wstring stemp = std::wstring(path.begin(), path.end());
	ShellExecute(0, 0, stemp.c_str(), 0, 0, SW_SHOW);
}

std::vector<std::string> FileTools::getAllImagePaths(const std::string& path)
{
	std::vector<std::string> content;

	for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
	{
		if (fs::is_regular_file(*it))
		{
			string ext = it->path().extension().string();

			if (!isImageExtension(ext))
			{
				continue;
			}

			string filePath = path + it->path().filename().string();
			content.push_back(filePath);
		}
	}

	return content;
}

std::string FileTools::getVideoPath(const std::string& path)
{
	std::string filePath = "";

	for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
	{
		if (fs::is_regular_file(*it))
		{
			string ext = it->path().extension().string();

			if (isVideoExtension(ext))
			{
				filePath = path + it->path().filename().string();
				break;
			}
		}
	}

	return filePath;
}
