#pragma once

#include "ISettings.h"
#include "KubikException.h"
#include "FileTools.h"
#include "ConfigTexts.h"
#include "JsonTools.h"
#include "ApplicationModel.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PhotoboothSettings> PhotoboothSettingsRef;

		class PhotoboothSettings : public ISettings
		{
		public:
			enum PhtTextID
			{
				PHOTO_OVER, PHOTO_OVER_SUB,
				CARD_STYLE, CARD_STYLE_SUB,
				FILTERS, FILTERS_SUB,
				PUBLISHING, PUBLISHING_SUB,
				SAVE_TEXT, YOUR_DESIGN_TEXT,
				VKONTAKTE, FACEBOOK, TWITTER,
				QRCODE, PRINTER, EMAIL, FILTER_TEXT1, FILTER_TEXT2,
				TIMER_TEXT1, TIMER_TEXT2,
				TEMPLATE_TEXT1, TEMPLATE_TEXT2, TEMPLATE_PRINT,
				PRELOAD_TEXT1, PRELOAD_TEXT2,
				CHOOSE_TEXT1, CHOOSE_TEXT2, PHOTO_FILTERS, RESHOT,
				SHARE_TITLE, QR_TEXT, EMAIL_TEXT, FB_TEXT, TW_TEXT, VK_TEXT, PLAY_AGAIN, ANOTHER_GAMES
			};

			class PhotoCountItem
			{
				gl::Texture texture;
				bool isActive;

			public:
				bool getActive() const;
			};

			PhotoboothSettings(ApplicationModelRef model, ConfigSettingsRef configSettings);

			void buildSettingData() override;
			void buildLocationData() override;	
			void load()			override;
			void setTextures()  override;
			fs::path getPhotoDownloadDirectory(){ return ci::app::getAppPath() / "photoDir"; };
			vector<Texture> getSmileTextures();

			TextItem getMainTitle(PhtTextID id);
			TextItem getSubTitleClose(PhtTextID id);
			TextItem getSubTitleOpen(PhtTextID id);
			TextItem getTextItem(PhtTextID id);
			bool	 getSocialState(PhtTextID id);
			void	 setSocialState(PhtTextID id, bool value);

			ci::gl::Texture getIcon(PhtTextID id);
			ci::gl::Texture getEmptyIcon();

			std::vector<ci::gl::Texture> getPhotoCardStylesActiveTemplate();
			std::vector<ci::gl::Texture> getPhotoOverActiveTemplate();

			ci::gl::Texture getActivePrintBgTex();
			void swapFilter(int id);
			
			DesignData getPhotoOverDesignData() const;
			DesignData getPhotoCardStyles() const;
			DesignData getPhotoFiltersPreview() const;

			void setActiveOverDesignID(int id);
			int getActiveOverDesignID();

			int getUserOverDesignID();

			int getActivePhotoCardStyleDesignID();
			void setActivePhotoCardStyleDesignID(int id);

			int getUserPhotoCardStyleDesignID();
		
			std::string getUserPhotoOverDesignPath();
			std::string getUserPhotoCardStylePath();

			void createMemento();
			void writeConfig();

			int getBeReadySeconds(){ return 5; };

			bool wasChanged(){ return false; };
			bool settingsChanged();
			changeSetting::id getChangeID() const { return changeSetting::id::PHOTOBOOTH; };

		public:
			class Filter
			{
				int id;
				bool isOn;
				std::string text;

			public:
				friend PhotoboothSettings;

				bool isActive() const;
				int getID() const;
				std::string getText() const;
			};

			std::vector<Filter> getFilters() const;
			std::vector<Filter> getOnFilters();

		private:
			class ImageElement
			{
				std::string path;
				ci::gl::Texture tex;
				int id;

			public:
				friend PhotoboothSettings;
			};

			typedef ImageElement Sticker;
			typedef ImageElement BackgroundPrint;

			class ConfigPath
			{
				std::string userStickerPath;
				std::string userCardStylePath;
				std::string photoOverDesignDataPath;
				std::string photoCardsStylesDesignDataPath;
				std::string photoFiltersPreviewDesignDataPath;
				std::string finalPath;

			public:
				friend PhotoboothSettings;
			};

			class Sharing
			{
			public:
				void setSocialState(PhtTextID id, bool state);
				void setIcon(ci::gl::Texture icon, PhtTextID id);
				void setEmptyIcon(ci::gl::Texture icon);

				bool getSocialState(PhtTextID id);
				ci::gl::Texture getEmptyIcon();
				ci::gl::Texture getIcon(PhtTextID id);

				friend PhotoboothSettings;

			private:
				std::map <PhtTextID, bool> states;
				std::map <PhtTextID, ci::gl::Texture> icons;
				ci::gl::Texture emptyIcon;
			};

			ConfigObject mainConfigObj;

			bool isSticker;
			int activeOverDesignID, activeOverDesignIDMemento;
			int userOverDesignID;
			int activePhotoCardStyleDesignID, activePhotoCardStyleDesignIDMemento;
			int userPhotoCardStyleDesignID;

			vector<ci::RectT<int>> photoCardStylesCoordRects, photoOverCoordRects;

			ConfigPath					 configPaths;
			Sharing						 sharing, sharingMemento;

			bool						 memento;

			std::vector<Filter>			 filters, filtersMemento;
			std::vector<Sticker>		 stickers;
			std::vector<BackgroundPrint> bgPrint;
			ConfigTexts<PhtTextID>		 configTexts;
			BackgroundPrint				 activeBgPrint;
			Sticker						 activeSticker;

			void loadPaths();
			void loadParams();
			void loadLabels();
			void loadConsts();
			void setDesignPath();

			void parsePhotoOverDesigns();
			void parsePhotoCardStyles();
			void parsePhotoFiltersPreview();

			void loadPhotoParams(JsonTree config);
			void loadSocialParams(JsonTree config);
			void loadPhotoFilterParams(JsonTree config);
			void loadGameDesignParams(JsonTree config);
			void loadGameStickerParams(JsonTree config);
			void loadConfigTexts(JsonTree config);
			void loadSharingIcons(JsonTree config);
			void saveConfig();
			void findAllImagePrints(std::string path, std::vector<ImageElement> &prints, bool isCustom);

			/////////////////////////////////

			DesignData photoOverDesignData;
			DesignData photoCardStyles;
			DesignData photoFiltersPreview;
			std::vector<std::string> smilePaths;
			std::vector<ci::gl::Texture> smileTextures;			
			std::string getActiveOverDesignText();
			std::string getActiveCardStyleText();
			std::string getActiveFiltersTexts();
			std::string getActivePublishingTexts();

			bool sharingNotEqual(Sharing sharing1, Sharing sharing2);
			bool filtersNotEqual(const std::vector<Filter>& filter1, const std::vector<Filter>& filter2);

			ConfigSettingsRef configSettings;
		};

		typedef PhotoboothSettings::PhtTextID  PhtTextID;
	}
}