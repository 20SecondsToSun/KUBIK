#pragma once

#include "cinder/app/AppNative.h"
#include "Types.h"
#include "cinder/Json.h"
#include "ApplicationModel.h"

namespace kubik
{
	class ISettings
	{
	public:

		std::string getDesignPath()
		{
			return getBasePath().string() + designPath;
		}

		std::string getStaticDesignPath(string value = "")
		{
			
			return getBasePath().string() + staticDesignPath + value;
		}

		std::string getTemplateDesignPath(string value = "")
		{
			return getBasePath().string() + templateDesignPath + value;
		}

		static std::string getFontsPath(string value = "")
		{
			ci::fs::path path = getBasePath()/"data/fonts/";
			return path.string() + value;
		}

		ImageResourceDictionary getTextures()
		{	
			return textures;
		}

		FontResourceDictionary getFonts()
		{	
			return fonts;
		}

		VideoResourceDictionary getVideos()
		{	
			return videos;
		}

		IResourceDictionary getResources()
		{	
			return resources;
		}

		void clearResources()
		{
			textures.clear();
			fonts.clear();
			videos.clear();
			resources.clear();
		}

		virtual void load() = 0;
		virtual void setTextures() = 0;	

	protected:		
		shared_ptr<ApplicationModel> model;	

		std::string configPath;

		std::string designPath;
		std::string staticDesignPath;
		std::string templateDesignPath;

		std::string fontsPath;

		ImageResourceDictionary textures;
		FontResourceDictionary fonts;
		VideoResourceDictionary videos;
		IResourceDictionary	resources;

		static ci::fs::path getBasePath()
		{
			ci::fs::path basePath = ci::app::getAppPath();
			return basePath;
		}		

		void addToDictionary(string key, shared_ptr<ImageResource>  value)
		{			
			textures[key] = value;
			resources[key] = value;
		}

		void addToDictionary(string key, shared_ptr<FontResource>  value)
		{
			fonts[key] = value;
			resources[key] = value;
		}

		void addToDictionary(string key, shared_ptr<VideoResource>  value)
		{
			videos[key] = value;
			resources[key] = value;
		}		

		shared_ptr<ImageResource> createImageResource( string path, loadingType loadType = loadingType::FULL_PATH)
		{
			shared_ptr<ImageResource> value	= shared_ptr<ImageResource>(new ImageResource);
			value->path				= path;			
			value->loadingType		= loadType;	

			return value;
		}

		shared_ptr<FontResource> createFontResource( string path, float size = 30, loadingType loadType = loadingType::FULL_PATH)
		{
			shared_ptr<FontResource> value	= shared_ptr<FontResource>(new FontResource);
			value->path				= path;			
			value->fontSize			= size;	
			value->loadingType		= loadType;	

			return value;
		}

		shared_ptr<VideoResource> createVideoResource( string path, loadingType loadType = loadingType::FULL_PATH)
		{
			shared_ptr<VideoResource> value	= shared_ptr<VideoResource>(new VideoResource);
			value->path				= path;
			value->loadingType		= loadType;	

			return value;
		}
	};
}