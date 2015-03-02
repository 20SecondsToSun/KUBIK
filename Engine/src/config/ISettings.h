#pragma once

#include "Types.h"
#include "ApplicationModel.h"

namespace kubik
{
	class ISettings
	{
	public:

		ISettings(ApplicationModelRef model):model(model)
		{

		}

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
			ci::fs::path path = getBasePath()/"data\\fonts\\";
			return path.string() + value;
		}

		static std::string getInterfacePath(string value = "")
		{
			ci::fs::path path = getBasePath()/"data\\interface\\";
			return path.string() + value;
		}

		ImageResourceDictionary getTextures()
		{	
			return textures;
		}

		Texture getTexture(const string& name)
		{	
			return textures[name]->get();
		}

		FontResourceDictionary getFonts()
		{	
			return fonts;
		}

		Font getFont(const string& name)
		{	
			return fonts[name]->get();
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
		virtual void buildData(){};
		virtual void createMemento() = 0;
		virtual void writeConfig() = 0;

	protected:		
		ApplicationModelRef model;	

		std::string mainConfigPath;

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

	typedef shared_ptr<ISettings> ISettingsRef;
}