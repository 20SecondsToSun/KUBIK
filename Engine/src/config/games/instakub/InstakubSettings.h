#pragma once

#include "ISettings.h"
#include "KubikException.h"
#include "FileTools.h"
#include "ConfigTexts.h"
#include "JsonTools.h"
#include "ApplicationModel.h"

namespace kubik
{
	namespace config
	{		
		typedef std::shared_ptr<class InstakubSettings> InstakubSettingsRef;
	
		class InstakubSettings: public ISettings
		{
		public:	
			enum InstaTextID
			{
				SEARCH_TITLE_MAIN, SEARCH_TITLE_SUB,
				HASHTAG_TITLE_MAIN,	HASHTAG_TITLE_SUB,
				PHOTO_TITLE_MAIN, PHOTO_TITLE_SUB				
			};

			InstakubSettings(ApplicationModelRef model);
			virtual void load();
			virtual void setTextures();
			virtual void buildData();

			void loadConfigPaths();
			void loadParams();
			void loadLabels();
			void parsePhotoCardStyles();
			
			TextItem getTextItem(const InstaTextID& id);

			bool getSearchFlag() const;
			void setSearchFlag(bool value);

			std::string getHashtag() const;
			void setHashtag(const std::string& value);

			DesignData getPhotoCardStyles() const;

			int getActivePhotoCardStyleDesignID() const;
			void setActivePhotoCardStyleDesignID(int id);

			int getUserPhotoCardStyleDesignID() const;
			std::string getUserPhotoCardStylePath() const;

			void writeConfig();
			void createMemento();
			bool settingsChanged();
			changeSetting::id getChangeID() const;

		private:
			bool memento;
			std::string hashtag, hashtag_save;
			bool search, search_save;
			int activePhotoCardStyleDesignID, activePhotoCardStyleDesignID_save;
			int userPhotoCardStyleDesignID;

			ConfigTexts<InstaTextID> configTexts;

			ConfigObject mainConfigObj;
			DesignData photoCardStyles;

			class ConfigPath
			{			
				std::string photoCardsStylesDesignDataPath;
				std::string userPhotoCardStylePath;
				
			public:
				friend InstakubSettings;
			}
			configPaths;
		};
	}
}