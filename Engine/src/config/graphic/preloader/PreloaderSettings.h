#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "KubikException.h"
#include "graphics/IMovie.h"
#include "MovieLoader.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PreloaderSettings> PreloaderSettingsRef;

		class PreloaderSettings :public ISettings
		{
		public:
			PreloaderSettings(ApplicationModelRef model, ConfigSettingsRef configSettings);
			
			virtual void load();
			void loadPaths();
			virtual void setTextures();
			virtual void createMemento(){};
			virtual void writeConfig(){};
			virtual bool settingsChanged(){ return false; };
			virtual changeSetting::id getChangeID() const ;	

			void buildData();
			
			IMovieRef getMainPreloader() const;
			IMovieRef getMiniPreloader() const;

		private:
			void setDesignPath();

			kubik::MovieLoader::MovieLoaderStruct miniPreloaderStruct;
			kubik::MovieLoader::MovieLoaderStruct mainPreloaderStruct;
			IMovieRef mainPreloader, miniPreloader;
			ConfigSettingsRef configSettings;

			std::string finalPath;
			ConfigObject mainConfigObj;
		};
	}
}