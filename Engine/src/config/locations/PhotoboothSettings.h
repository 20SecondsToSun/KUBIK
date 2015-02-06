#pragma once

#include "ISettings.h"
#include "KubikException.h"
#include "FileTools.h"
#include "ConfigTexts.h"
#include "JsonTools.h"

#include "ApplicationModel.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	namespace config
	{
		static const std::string STICKER_SUPPORT_EXTENSION = ".png";
		static const std::string CONFIG_FILE = "path.txt";
		static const std::string PARAMS_FILE = "params.txt";
		static const std::string LABELS_FILE = "labels.txt";

		static const std::string STICKER_NAME = "sticker";
		static const std::string PRINT_TEMPATE_NAME = "print_template";

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
				QRCODE,	PRINTER,EMAIL
			};

			struct Filter
			{
				int id;
				bool isOn;
			};

			struct ImageElement
			{
				std::string path;
				Texture tex;
				int id;
			};

			typedef ImageElement Sticker;
			typedef ImageElement BackgroundPrint;

			struct ConfigPath
			{
				std::string staticPartDesignPath;
				std::string kubikTemplatePartDesignPath;
				std::string userTemplatePartDesignPath;
				std::string finalPath;
				std::string stickersPath;		
				std::string bgPrintsPath;
			};		

			class Sharing
			{
			public:
				void setSocialState(PhtTextID id, bool state)
				{
					states[id] = state;				
				}

				bool getSocialState(PhtTextID id)
				{
					return states[id];
				}

				void setIcon(Texture icon, PhtTextID id)
				{
					icons[id] = icon;
				}

				Texture getIcon(PhtTextID id)
				{
					return icons[id];
				}

				void setEmptyIcon(Texture icon)
				{
					emptyIcon = icon;
				}

				Texture getEmptyIcon()
				{
					return emptyIcon;
				}

				private:
					std::map <PhtTextID, bool> states;
					std::map <PhtTextID, Texture> icons;
					Texture emptyIcon;
			};

			struct PhotoboothDataStruct
			{
				static const int MIN_PHOTO_SHOTS = 1;
				static const int MAX_PHOTO_SHOTS = 3;

				static const int MIN_SEC_BETWEEN_SHOTS = 1;
				static const int MAX_SEC_BETWEEN_SHOTS = 6;

				static const int MIN_COUNTDOWN_TIMER = 3;
				static const int MAX_COUNTDOWN_TIMER = 5;

				Sharing sharing;

				int  seconds;
				int  secondsBetweenShots;
				int  photoNum;
				int  templateId;
				bool isCustomDesign;
				bool isSticker;

				std::vector<Filter>			 filters;		
				std::vector<Sticker>		 stickers;
				std::vector<BackgroundPrint> bgPrint;

				BackgroundPrint activeBgPrint;
				Sticker			activeSticker;

				bool hasDesignChanges(PhotoboothDataStruct val)
				{
					return (val.isCustomDesign != isCustomDesign ||
						val.templateId	   != templateId);
				}

				bool hasAnyChanges(PhotoboothDataStruct val)
				{
					return (val.sharing.getSocialState(PhtTextID::EMAIL)	 != sharing.getSocialState(PhtTextID::EMAIL) ||
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
						val.activeBgPrint.id		  != activeBgPrint.id);
				}

				ConfigTexts<PhtTextID> configTexts;
				ConfigTexts<PhtTextID>& getTexts()
				{
					return configTexts;
				}

				void setTexts(ConfigTexts<PhtTextID>& val)
				{
					configTexts.setDic(val.getDic());
				}	

				//std::map <string, ConfigTitles> mainTitles, subTitles;

				//void setMainTitles(string lang, ConfigTitles txts)
				//{
				//	mainTitles[lang] = txts;			
				//}

				//void setSubTitles(string lang, ConfigTitles txts)
				//{
				//	subTitles[lang] = txts;			
				//}

				//void setText(std::pair<TextID, string> key, string txts)
				//{
				//	texts[key].set(txts);
				//}

				//string getText(std::pair<TextID, string> key)
				//{
				//	return texts[key].get();
				//}
				//std::map <std::pair<PhtTextID, string>, StringVO> texts;
			};

			TextItem getMainTitle(PhtTextID id)
			{
				return data.getTexts().get(model->getLang(), id);//data.getTexts().get(model->getLang(), id);
			}

			TextItem getSubTitleClose(PhtTextID id)
			{
				return data.getTexts().get(model->getLang(), static_cast<PhtTextID>(int(id) + 1));
			}

			TextItem getSubTitleOpen(PhtTextID id)
			{
				return data.getTexts().get(model->getLang(), static_cast<PhtTextID>(int(id) + 1));
			}

			TextItem  getTextItem(PhtTextID id)	
			{
				return data.getTexts().get(model->getLang(), id);
			}		

			bool getSocialState(PhtTextID id)
			{
				return data.sharing.getSocialState(id);
			}

			void setSocialState(PhtTextID id, bool value)
			{
				data.sharing.setSocialState(id, value);
			}

			Texture getIcon(PhtTextID id)
			{
				return data.sharing.getIcon(id);
			}

			Texture getEmptyIcon()
			{
				return data.sharing.getEmptyIcon();
			}

			PhotoboothSettings(shared_ptr<ApplicationModel> model);
			void buildData() override;

			void load() override;
			void setTextures() override;

			Texture getActiveStickerTex();
			Texture getActivePrintBgTex();

			bool findFilterId(int id, std::vector<int> filters);		

			std::vector<int> getOnFilters();
			int getPhotoShots();
			void setDesignPath();

			PhotoboothDataStruct getData();
			void setData(PhotoboothDataStruct value);		

		private:
			PhotoboothDataStruct data;
			ConfigPath configPaths;

			void setConfigPaths();
			void setParams();
			void setPhotoParams(JsonTree config);
			void setSocialParams(JsonTree config);
			void setPhotoFilterParams(JsonTree config);
			void setGameDesignParams(JsonTree config);
			void setGameStickerParams(JsonTree config);
			void setGameBgPrintParams(JsonTree config);
			void setConfigData(JsonTree config);
			void setSharingIcons(JsonTree config);
			void saveConfig();		
			void findAllImagePrints(string path, std::vector<ImageElement> &prints, bool isCustom);
		};

		typedef std::shared_ptr<PhotoboothSettings> PhotoboothSettingsRef;
		typedef PhotoboothSettings::PhtTextID  PhtTextID;
	}
}