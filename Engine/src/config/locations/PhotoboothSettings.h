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
		typedef std::shared_ptr<class PhotoboothSettings> PhotoboothSettingsRef;
	
		class PhotoboothSettings: public ISettings
		{
		public:	
			enum PhtTextID
			{
				PHOTO_OVER,	PHOTO_OVER_SUB,
				CARD_STYLE,	CARD_STYLE_SUB,
				PRINT_COUNT, PRINT_COUNT_SUB,
				FILTERS, FILTERS_SUB,
				PUBLISHING,	PUBLISHING_SUB,
				SAVE_TEXT, YOUR_DESIGN_TEXT,
				VKONTAKTE, FACEBOOK, TWITTER,
				QRCODE,	PRINTER,EMAIL, PHOTO_TREMPLATE_1, PHOTO_TREMPLATE_2
			};

			class PhotoCountItem
			{
				gl::Texture texture;
				bool isActive;

			public:
				bool getActive()
				{
					return isActive;
				}
			};

			PhotoboothSettings(ApplicationModelRef model);

			void buildData()    override;
			void load()			override;
			void setTextures()  override;			

			bool hasDesignChanges()
			{
				//compare with memento
				//return (val.isCustomDesign != isCustomDesign ||
				//	val.templateId	   != templateId);
				return false;
			}

			bool hasAnyChanges(/*PhotoboothDataStruct val*/)
			{
				return false;/* (val.sharing.getSocialState(PhtTextID::EMAIL)	 != sharing.getSocialState(PhtTextID::EMAIL) ||
							 val.sharing.getSocialState(PhtTextID::FACEBOOK)	 != sharing.getSocialState(PhtTextID::FACEBOOK) ||
							 val.sharing.getSocialState(PhtTextID::PRINTER)	 != sharing.getSocialState(PhtTextID::PRINTER)||
							 val.sharing.getSocialState(PhtTextID::QRCODE)	 != sharing.getSocialState(PhtTextID::QRCODE)||					
							 val.sharing.getSocialState(PhtTextID::TWITTER)	 != sharing.getSocialState(PhtTextID::TWITTER)||
							 val.sharing.getSocialState(PhtTextID::VKONTAKTE) != sharing.getSocialState(PhtTextID::VKONTAKTE)||
							 val.isSticker			      != isSticker ||
							 val.photoNum				  != photoNum ||
							 val.seconds					  != seconds ||
							 val.secondsBetweenShots		  != secondsBetweenShots ||
							 val.activeSticker.id		  != activeSticker.id ||					
							 val.activeBgPrint.id		  != activeBgPrint.id);	*/		
			}			

			TextItem getMainTitle(PhtTextID id);
			TextItem getSubTitleClose(PhtTextID id);
			TextItem getSubTitleOpen(PhtTextID id);
			TextItem getTextItem(PhtTextID id);
			bool	 getSocialState(PhtTextID id);
			void	 setSocialState(PhtTextID id, bool value);

			ci::gl::Texture getIcon(PhtTextID id);
			ci::gl::Texture getEmptyIcon();
			ci::gl::Texture getActiveStickerTex();
			ci::gl::Texture getActivePrintBgTex();
			std::vector<int> getOnFilters();
			int getPhotoShots();
			bool findFilterId(int id, std::vector<int> filters);	

			int getPhotoCount(PhtTextID id);	

			DesignData getPhotoOverDesignData()
			{
				return photoOverDesignData;
			}
			DesignData getPhotoCardStyles()
			{
				return photoCardStyles;
			}

		private:
			class Filter
			{
				int id;
				bool isOn;

			public:
				friend PhotoboothSettings;
			};

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
				std::string staticPartDesignPath;
				std::string kubikTemplatePartDesignPath;
				std::string userTemplatePartDesignPath;
				std::string finalPath;
				std::string stickersPath;		
				std::string bgPrintsPath;
				std::string photoOverDesignDataPath;
				std::string photoCardsStylesDesignDataPath;

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

			int seconds;
			int secondsBetweenShots;
			int photoNum, photoNumMax, photoNumMin;
			int templateId;			
			int minPhotosShots;
			int maxPhotosShots;		
			int minSecBetweenShots;
			int maxSecBetweenShots;			
			int minCountTimer;
			int maxCountTimer;
			bool isCustomDesign;
			bool isSticker;
	
			ConfigPath					 configPaths;
			Sharing						 sharing;
			std::vector<Filter>			 filters;		
			std::vector<Sticker>		 stickers;
			std::vector<BackgroundPrint> bgPrint;
			ConfigTexts<PhtTextID>		 configTexts;
			BackgroundPrint				 activeBgPrint;
			Sticker						 activeSticker;

			void loadConfigPaths();
			void loadParams();
			void loadLabels();			
			void loadConsts();			
			void loadDesignPath();
			
			void parsePhotoOverDesigns();
			void parsePhotoCardStyles();

			void loadPhotoParams(JsonTree config);
			void loadSocialParams(JsonTree config);
			void loadPhotoFilterParams(JsonTree config);
			void loadGameDesignParams(JsonTree config);
			void loadGameStickerParams(JsonTree config);
			void loadGameBgPrintParams(JsonTree config);
			void loadConfigTexts(JsonTree config);
			void loadSharingIcons(JsonTree config);
			void saveConfig();		
			void findAllImagePrints(string path, std::vector<ImageElement> &prints, bool isCustom);	

			/////////////////////////////////

			DesignData photoOverDesignData;		
			DesignData photoCardStyles;
						
		};	
		typedef PhotoboothSettings::PhtTextID  PhtTextID;
	}
}