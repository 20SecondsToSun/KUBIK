#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class PhotoStorage
	{
		vector<string> photoDownloadedPaths;
		vector<Surface> screenshots;
		vector<Surface> hiResPhotos;

	public:
		void clear()
		{
			photoDownloadedPaths.clear();
			screenshots.clear();
			hiResPhotos.clear();
		}

		vector<string> getPhotoPaths() 
		{
			return photoDownloadedPaths;
		}

		void setPhotoPath(const string& downloadedPath) 
		{
			photoDownloadedPaths.push_back(downloadedPath);
		}	

		void addScreenshot(const Surface& surface) 
		{
			screenshots.push_back(surface);
		}

		void setHiResPhoto(const Surface& surface) 
		{
			hiResPhotos.push_back(surface);
		}

		vector<Surface> getHiResPhoto() 
		{
			return hiResPhotos;
		}
	};
}