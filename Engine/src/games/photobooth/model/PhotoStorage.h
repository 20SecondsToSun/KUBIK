#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ip/Resize.h"
#include "Utils.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class PhotoSampler
	{
		string pathHiRes;
		Surface screenshot;
		Surface hiResSource;
		bool isLoading;
		int id;

	public:
		PhotoSampler(int _id):id(_id), isLoading(false)
		{
			
		}

		string getHiResPath() 
		{
			return pathHiRes;
		}

		void setHiResPath(const string& path) 
		{
			pathHiRes = path;
		}

		Surface getScreenshot() 
		{
			return screenshot;
		}

		void setScreenshot(const Surface& surface) 
		{
			screenshot = surface;
		}

		Surface getHiResPhoto() 
		{
			return hiResSource;
		}

		void setHiResPhoto(const Surface& surface) 
		{
			hiResSource = surface;
			isLoading = true;
		}

		Surface resize(const int& width) 
		{
			Surface source;

			if(isLoading)
				source = hiResSource;
			else
				source = screenshot;

			double scale = (double)width / source.getWidth();
			Surface thumb = ci::Surface( width, (int)(source.getHeight() * scale), false); 
			ci::ip::resize(source, source.getBounds(), &thumb, thumb.getBounds(), ci::FilterBox());

			return thumb;
		}
	};

	typedef shared_ptr<PhotoSampler> PhotoSamplerRef;

	class PhotoStorage
	{
		map<int, PhotoSamplerRef> photos;
		vector<Surface> choosingThumbs;
		vector<Surface> sharingPhotos;		
		vector<int>	choosedIds;
		vector<Surface> printTemplates;
		
		bool threadIsRunning;
		ThreadRef loadingThread;
	public:

		PhotoStorage():threadIsRunning(false)
		{

		}

		map<int, PhotoSamplerRef> getPhotos()
		{
			return photos;
		}

		vector<Surface> getChoosingThumbs()
		{
			return choosingThumbs;
		}

		void prepare(int count)
		{
			console()<<"PREPARE::"<<endl;
			photos.clear();
			choosingThumbs.clear();
			choosedIds.clear();

			for (int i = 1; i <= count; i++)
				photos[i] = PhotoSamplerRef(new PhotoSampler(i));
		}

		string getPhotoPaths(int id) 
		{
			return photos[id]->getHiResPath();
		}

		void setPhotoPath(int id, const string& path) 
		{
			photos[id]->setHiResPath(path);
		}	

		void setScreenshot(int id, const Surface& surface) 
		{
			photos[id]->setScreenshot(surface);
		}

		Surface getScreenshot(int id) 
		{
			return photos[id]->getScreenshot();
		}

		void setHiResPhoto(int id, const Surface& surface) 
		{
			photos[id]->setHiResPhoto(surface);
		}

		Surface getHiResPhoto(int id)
		{
			return photos[id]->getHiResPhoto();
		}

		void prepareAllResizes()
		{
			int shift = 20;
			int size = photos.size();
		    int width = getWindowWidth() / size - shift;

			for (int i = 1; i <= size; i++)
			{
				Surface resized = photos[i]->resize(width);
				choosingThumbs.push_back(resized);
			}
		}

		void loadHiRes()
		{
			threadIsRunning = true;
			loadingThread = ThreadRef(new boost::thread(&PhotoStorage::loadPhotos, this));
		}

		void loadPhotos() 
		{			
			for (auto sampler : photos)
			{
				string  path = sampler.second->getHiResPath();				
				try
				{				
					if(path != "")
					{
						Surface image = Surface(loadImage(loadFile(path)));
						sampler.second->setHiResPhoto(image);	
					}
				}
				catch(...)
				{
					
				}
			}

			threadIsRunning = false;
		}

		bool isHiResLoaded() const
		{
			return !threadIsRunning;
		}

		void clearPhotoChosenIds()
		{
			choosedIds.clear();
		}

		void setPhotosChoosenIds(int id)
		{
			choosedIds.push_back(id);
		}

		bool isPhotosChoosedEmpty() const
		{
			return choosedIds.empty();
		}

		bool isChoosedRightCount(int count) const
		{
			return choosedIds.size() == count;
		}

		void createSharingPhotos(Texture stickerTex)
		{			
			int size = photos.size();
		    int width = 1000;

			for (int i = 1; i <= size; i++)
			{
				Surface resized = photos[i]->resize(width);
				gl::Fbo mFbo = gl::Fbo(resized.getWidth(), resized.getHeight());
				drawAllInFinallFBO(mFbo, resized, stickerTex);
				sharingPhotos.push_back(Surface(mFbo.getTexture()));
				Utils::clearFBO(mFbo);
				writeImage( getAppPath()/"data"/"temp"/(to_string(i)+".jpg"), sharingPhotos[i-1]);
				console()<<"create:::::::::::"<<endl;
			}
		}

		void drawAllInFinallFBO(gl::Fbo& mFbo, Surface img, Texture sticker)
		{
			Utils::drawGraphicsToFBO(mFbo, [ & ]()
			{
				gl::pushMatrices();
				gl::clear(Color::black());
				gl::enableAlphaBlending();  
				gl::draw(img);			
				gl::draw(sticker);
				gl::popMatrices();
			});	
		}

		
		void createPrintTemplatePhoto(Texture printTemplateTex, Texture stickerTex)
		{
			printTemplates.clear();

			printTemplates.push_back(Surface(printTemplateTex));
			printTemplates.push_back(Surface(printTemplateTex));
			printTemplates.push_back(Surface(printTemplateTex));
		}		

		vector<Surface> getPrintTemplates()
		{			
			return printTemplates;
		}
	};

	typedef shared_ptr<PhotoStorage> PhotoStorageRef;
}