#pragma once

#include "config/ApplicationModel.h"
#include "ISettings.h"
#include "ConfigSettings.h"
#include "StatCollector.h"

#ifndef release
#define Poza_DEBUG
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
				ci::gl::Texture comicsTex;
				ci::gl::Texture conturTex;
				ci::JsonTree data;
				PozaDataObj(){};
				PozaDataObj(const ci::JsonTree& data, const std::string& contur, const std::string& comics)
				{
					this->data = data;
					this->contur = contur;
					this->comics = comics;
				}
			};
			std::vector<PozaSettings::PozaDataObj> pozaDataVec;// TODO private!!!

			static const float GoToScreenSaverTime;

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
		};
	}
}