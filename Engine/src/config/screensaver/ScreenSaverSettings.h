#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "FileTools.h"
#include "KubikException.h"
#include "IResourceScreenSaver.h"
#include "VideoScreenSaver.h"
#include "ImageScreenSaver.h"
#include "FileTools.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ScreenSaverSettings> ScreenSaverSettingsRef;

		class ScreenSaverSettings :public ISettings
		{
		public:			
			bool isActive, isActiveMemento, memento;
			std::string path;		

			bool getActive() const;
			void setActive(bool value);
			std::string getPath() const;

			ScreenSaverSettings(ApplicationModelRef model);

			void load() override;
			void saveConfig();	
			void setTextures() override;
			bool isExist() const;
			bool isShow() const;
			bool settingsChanged();
			int getScreenSaverMode() const;	
			
			virtual void createMemento();
			virtual void writeConfig();

			std::string getScreenSaverPath() const;
			std::shared_ptr<IResourceScreenSaver> getResource();

			changeSetting::id getChangeID() const;

		private:
			enum { IMAGE_SS, VIDEO_SS, NONE_SS };
			int mode;
			std::string path_ss;
			std::shared_ptr<IResourceScreenSaver> screenSaverResource;

			void findScreenSaver();
			int getContentType(const std::string& ext);
			bool fileSizeNotTooBig(const fs::path& filePath, const std::string& ext);
		};		
	}
}