#pragma once

#include "ISettings.h"
#include "KubikException.h"
#include "FileTools.h"
#include "ConfigTexts.h"
#include "JsonTools.h"
#include "ApplicationModel.h"
#include "ConfigSettings.h"
#include "graphics/IMovie.h"
#include "fontStorage/FontStorage.h"
#include "Paths.h"

#define Photobooth_DEBUG
#define Photobooth_Loadfromfolder_DEBUG
//#define Photobooth_Sharing_DEBUG

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class PhotoboothSettings> PhotoboothSettingsRef;

		class PhotoboothSettings : public ISettings
		{
		public:
			static const float GoToScreenSaverTime;

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

			enum class PhotoFormat
			{
				FORMAT1,
				FORMAT2,
				FORMAT3,
				FORMAT4,
				FORMAT5
			};

			typedef std::pair<int, PhotoFormat>  StickerKey;
			typedef std::pair<int, PhotoFormat>  PhotoCardKey;
			typedef std::map<StickerKey, ci::gl::Texture> StickerMap;
			typedef std::map<PhotoCardKey, ci::gl::Texture> PhotoCardsMap;			
			
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
			fs::path getPhotoDownloadDirectory(){ return kubik::Paths::getPhotoDownloadedPath(); };

			TextItem getMainTitle(const PhtTextID& id);
			TextItem getSubTitleClose(const PhtTextID& id);
			TextItem getSubTitleOpen(const PhtTextID& id);
			TextItem getTextItem(const PhtTextID& id);
			bool	 getSocialState(const PhtTextID& id);
			void	 setSocialState(const PhtTextID& id, bool value);

			ci::gl::Texture getIcon(const PhtTextID& id);
			ci::gl::Texture getEmptyIcon() const;

			std::vector<ci::gl::Texture> getSmileTextures() const;
			std::vector<ci::gl::Texture> getStickerTextures() const;
			std::vector<ci::gl::Texture> getPhotoCardsTextures() const;

			ci::gl::Texture getPhotoShootingCard() const;
			ci::gl::Texture getPhotoSharingCard() const;			
			ci::gl::Texture getActivePrintBgTex();
			void swapFilter(int id);
			
			DesignData getPhotoOverDesignData() const;
			DesignData getPhotoCardStyles() const;
			DesignData getPhotoFiltersPreview() const;
			
			int getActiveOverDesignID() const;
			int getUserOverDesignID() const;
			int getActivePhotoCardStyleDesignID() const;
			int getUserPhotoCardStyleDesignID() const;

			void setActiveOverDesignID(int id);
			void setActivePhotoCardStyleDesignID(int id);			
			void createMemento();
			void writeConfig();

			std::string getUserPhotoOverDesignPath();
			std::string getUserPhotoCardStylePath();			

			int getBeReadySeconds(){ return 5; };
			
			changeSetting::id getChangeID() const { return changeSetting::id::PHOTOBOOTH; };

			IMovieRef getPreloader();

			bool wasChanged(){ return false; };
			bool settingsChanged();
			bool isPrinterOn();
			bool onlyOneGameOn();
		
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
			static const int CARDS_COUNT = 5;
			static const int STICKERS_COUNT = 5;			

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
				void setSocialState(const PhtTextID& id, bool state);
				void setIcon(const ci::gl::Texture& icon, const PhtTextID& id);
				void setEmptyIcon(const ci::gl::Texture& icon);

				bool getSocialState(const PhtTextID& id);
				ci::gl::Texture getEmptyIcon() const;
				ci::gl::Texture getIcon(const PhtTextID& id);

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
			
			ConfigPath					 configPaths;
			Sharing						 sharing, sharingMemento;
			bool						 memento;
			
			StickerMap stickersImagesMap;
			PhotoCardsMap photoCardsImagesMap;

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

			void loadPhotoParams(const JsonTree& config);
			void loadSocialParams(const JsonTree& config);
			void loadPhotoFilterParams(const JsonTree& config);
			void loadGameDesignParams(const JsonTree& config);
			void loadGameStickerParams(const JsonTree& config);
			void loadConfigTexts(const JsonTree& config);
			void loadSharingIcons(const JsonTree& config);
			void saveConfig();
			void findAllImagePrints(std::string path, std::vector<ImageElement> &prints, bool isCustom);
			
			/////////////////////////////////

			DesignData stickersDesignData;
			DesignData cardsBackgroundDesignData;
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

			std::string getPhotoCardName(int i, int j) const;
			std::string getPhotoCardPath(const OneDesignItem& photoCard, int j) const;
		};

		typedef PhotoboothSettings::PhtTextID  PhtTextID;
	}
}
