#pragma once

#include "config/ApplicationModel.h"
#include "ISettings.h"
#include "ConfigSettings.h"
#include "StatCollector.h"

#ifndef release
#define Poza_DEBUG
#define Poza_Sharing_DEBUG
#endif

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PozaSettings> PozaSettingsRef;

		class PozaSettings : public ISettings, public StatCollector
		{
			ConfigObject mainConfigObj;
			ConfigSettingsRef configSettings;

			std::string finalPath;
			std::string pozaDataPath;
			std::string pozaBaseFileName;
			std::string pozaCardsConfigPath;
			int activeCardDesignID, userCardDesignID;
			DesignData photoCardStyles;

			void setDesignPath();
			void loadPaths();
			void loadParams();		
			
		public:
			struct PozaDataObj
			{
				std::string contur;
				std::string comics;
				std::string path;
				std::string data;
				ci::gl::Texture comicsTex;
				ci::gl::Texture conturTex;
				std::vector<ci::Vec2f> points;
				PozaDataObj();
				PozaDataObj(const std::string& path, const std::string& data, const std::string& contur, const std::string& comics);				
			};
			std::vector<PozaSettings::PozaDataObj> pozaDataVec;// TODO private!!!

			static const float GoToScreenSaverTime;
			static const float DISTANCE_OK;

			PozaSettings(ApplicationModelRef model, ConfigSettingsRef configSettings);

			virtual void createMemento();
			virtual void writeConfig();
			virtual void buildLocationData();

			void load() override;
			void setTextures() override;			
			bool settingsChanged();

			ci::gl::Texture getCurrentTemplate();
			changeSetting::id getChangeID() const;

			bool onlyOneGameOn(){ return false; };
			ci::fs::path getPhotoDownloadDirectory(){ return Paths::getPhotoDownloadedPath(); };

			bool getSocialState(int value){ return true; }

			std::vector<PozaSettings::PozaDataObj> getPozaData() const
			{
				return pozaDataVec;
			}

			virtual std::string getDataBasePath() const override;
			virtual int getAppID() override{ return game::id::POZA; };
		};
	}
}