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
#include "fontStorage/FontStorage.h"
#include "StatCollector.h"

namespace kubik
{
	namespace config
	{		
		typedef std::shared_ptr<class InstakubSettings> InstakubSettingsRef;
	
		class InstakubSettings: public ISettings, public StatCollector
		{
		public:	
			enum InstaTextID
			{
				SEARCH_TITLE_MAIN, SEARCH_TITLE_SUB,
				HASHTAG_TITLE_MAIN,	HASHTAG_TITLE_SUB,
				PHOTO_TITLE_MAIN, PHOTO_TITLE_SUB				
			};	

			static const int TimeForReload = 60; // seconds
			static const int TtimeForPrinting = 23; // seconds

			InstakubSettings(ApplicationModelRef model, ConfigSettingsRef configSettings);
			void load() override;
			void setTextures() override;
			void buildLocationData() override;
			void buildSettingData() override;
			
			void loadPaths();
			void loadParams();
			void loadLabels();
			void loadConsts();			
			void setDesignPath();
			void parsePhotoCardStyles();

			ci::gl::Texture getCurrentTemplate();
			ci::ColorA getKeyboardColorInDesign();
			
			TextItem getTextItem(const InstaTextID& id);

			bool getSearchFlag() const;
			void setSearchFlag(bool value);

			TextItem getHashtag() const;
			void setHashtagText(const std::string& value);

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

			ci::ColorA getPreloaderToneColor() const;
			ci::Font getViewInputFieldFont();

			std::string getDataBasePath() const;
			std::string getDataBaseName(const std::string add = "") const;

			// statistics
			void savePrintInstaLink(const std::string& saveData);
			void saveSearchInstaLink(const std::string& saveData);

			virtual int getAppID() override{ return game::id::INSTAKUB; };

		private:			
			std::string hashtag, hashtag_save, clientID;
			std::string viewInputFieldFontName;
			bool memento, search, search_save;
			int activeCardDesignID, activeCardDesignID_save;
			int userCardDesignID;
			std::vector<ci::ColorA> keyboardColorsInDesign;
			ConfigTexts<InstaTextID> configTexts;
			ConfigObject mainConfigObj;
			ConfigSettingsRef configSettings;
			DesignData photoCardStyles;

			class ConfigPath
			{			
				std::string photoCardsStylesDesignDataPath;
				std::string userPhotoCardStylePath;
				std::string finalPath;				
				
			public:
				friend InstakubSettings;
			}
			configPaths;

			TextItem hashTagTextItem;
			ci::ColorA preloaderToneColor;			
		};
	}
}