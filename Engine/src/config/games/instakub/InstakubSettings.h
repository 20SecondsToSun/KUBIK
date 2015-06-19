#pragma once

#include "ISettings.h"
#include "KubikException.h"
#include "FileTools.h"
#include "ConfigTexts.h"
#include "JsonTools.h"
#include "ApplicationModel.h"
#include "ConfigSettings.h"
#include "MovieLoader.h"
#include "ImageSequencer/ImageSequencer.h"
#include "videoplayer/VideoPlayer.h"

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

			InstakubSettings(ApplicationModelRef model, ConfigSettingsRef configSettings);
			void load() override;
			void setTextures() override;
			void buildLocationData() override;
			void buildSettingData() override;

			ci::gl::Texture getCurrentTemplate();

			void loadPaths();
			void loadParams();
			void loadLabels();
			void loadConsts();			
			void setDesignPath();
			void parsePhotoCardStyles();
			ci::ColorA getKeyboardColorInDesign();
			
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
			std::string getClientID() const;

			void writeConfig();
			void createMemento();
			bool settingsChanged();
			changeSetting::id getChangeID() const;

			bool hashtagEnabled() const;
			bool searchEnabled() const;

		private:			
			bool memento;
			std::string hashtag, hashtag_save, clientID;
			bool search, search_save;
			int activePhotoCardStyleDesignID, activePhotoCardStyleDesignID_save;
			int userPhotoCardStyleDesignID;

			std::vector<ci::ColorA> keyboardColorsInDesign;

			ConfigTexts<InstaTextID> configTexts;

			ConfigObject mainConfigObj;
			DesignData photoCardStyles;

			ConfigSettingsRef configSettings;

			class ConfigPath
			{			
				std::string photoCardsStylesDesignDataPath;
				std::string userPhotoCardStylePath;
				std::string finalPath;				
				
			public:
				friend InstakubSettings;
			}
			configPaths;
		};
	}
}