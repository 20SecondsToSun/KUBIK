#pragma once
#include "Types.h"
#include "ApplicationModel.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ISettings> ISettingsRef;

		class ISettings
		{
		public:
			ISettings(ApplicationModelRef model);

			static std::string getInterfacePath(const std::string& value = "");

			std::string getDesignPath() const;
			std::string getStaticDesignPath(const std::string& value = "")   const;
			std::string getTemplateDesignPath(const std::string& value = "") const;

			ImageResourceDictionary getTextures() const;
			VideoResourceDictionary getVideos() const;
			IResourceDictionary getResources() const;
			IResourceDictionary getSettingsResources() const;

			ci::gl::Texture getTexture(const std::string& name);
		
			void clearResources();

			virtual void buildLocationData();
			virtual void buildSettingData();		

			virtual void load() = 0;
			virtual void setTextures() = 0;			
			virtual void createMemento() = 0;
			virtual void writeConfig() = 0;
			virtual bool settingsChanged() = 0;
			virtual changeSetting::id getChangeID() const = 0;

		protected:
			ApplicationModelRef model;

			settings::id settingsID;

			std::string mainConfigPath;
			std::string designPath;
			std::string staticDesignPath;
			std::string templateDesignPath;
			std::string fontsPath;

			ImageResourceDictionary textures;
			VideoResourceDictionary videos;
			IResourceDictionary	resources, settingsResources;

			static ci::fs::path getBasePath();

			void addToDictionary(const std::string& key, std::shared_ptr<ImageResource>  value);
			void addToDictionary(const std::string& key, std::shared_ptr<FontResource>   value);
			void addToDictionary(const std::string& key, std::shared_ptr<VideoResource>  value);

			void addToSettingsDictionary(const std::string& key, std::shared_ptr<ImageResource>  value);
			void addToSettingsDictionary(const std::string& key, std::shared_ptr<FontResource>   value);
			void addToSettingsDictionary(const std::string& key, std::shared_ptr<VideoResource>  value);

			std::shared_ptr<ImageResource> createImageResource(const std::string& path,					 loadingType loadType = loadingType::FULL_PATH);
			std::shared_ptr<FontResource>  createFontResource( const std::string& path, float size = 30, loadingType loadType = loadingType::FULL_PATH);
			std::shared_ptr<VideoResource> createVideoResource(const std::string& path,					 loadingType loadType = loadingType::FULL_PATH);

		private:
			static const std::string InterfacePath;
		};
	}
}
