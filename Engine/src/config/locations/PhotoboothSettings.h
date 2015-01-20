#pragma once

#include "ISettings.h"
#include "KubikException.h"
#include "FileTools.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	static const string STICKER_SUPPORT_EXTENSION = ".png";
	static const string CONFIG_FILE = "path.txt";
	static const string PARAMS_FILE = "params.txt";
	static const string LABELS_FILE = "labels.txt";

	static const string STICKER_NAME = "sticker";
	static const string PRINT_TEMPATE_NAME = "print_template";

	class PhotoboothSettings: public ISettings
	{
	public:

		enum TextID
		{
			SAVE_TEXT,
			YOUR_DESIGN_TEXT
		};

		enum SettingsPartID
		{
			PHOTO_OVER,
			CARD_STYLE,
			PRINT_COUNT,
			FILTERS,
			SHARING		
		};

		enum SocialID
		{
			VKONTAKTE,
			FACEBOOK,
			TWITTER,
			QRCODE,
			PRINTER,
			EMAIL
		};

		struct Filter
		{
			int id;
			bool isOn;
		};

		struct ImageElement
		{
			string path;
			Texture tex;
			int id;
		};

		typedef ImageElement Sticker;
		typedef ImageElement BackgroundPrint;

		struct ConfigPath
		{
			string staticPartDesignPath;
			string kubikTemplatePartDesignPath;
			string userTemplatePartDesignPath;
			string finalPath;

			string stickersPath;		
			string bgPrintsPath;
		};

		class StringVO
		{
		public:
			StringVO(string str = ""):data(str)
			{
			}

			void set(string str)
			{
				data = str;
			}

			string get()
			{
				return data;
			}
		private:
				string data;
		};

		class Sharing
		{
		public:
			void setSocialTitle(std::pair<SocialID, string> key, StringVO txts)
			{
				titles[key] = txts;
			}

			string getSocialTitle(std::pair<SocialID, string> key)
			{
				return titles[key].get();
			}

			void setSocialState(SocialID id, bool state)
			{
				states[id] = state;
			}

			bool getSocialState(SocialID id)
			{
				return states[id];
			}

			void setIcon(Texture icon, SocialID id)
			{
				icons[id] = icon;
			}

			Texture getIcon(SocialID id)
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

			friend PhotoboothSettings;

		private:
			std::map <SocialID, bool> states;
			std::map <SocialID, Texture> icons;
			Texture emptyIcon;
			std::map <std::pair<SocialID, string>, StringVO> titles;
		};

		class ConfigTitles
		{
		public:
			void set(SettingsPartID id, string title)
			{
				titles[id] = title;
			}

			string get(SettingsPartID id)
			{
				return titles[id];
			}

		private:
			std::map <SettingsPartID, string> titles;
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

			vector<Filter>			filters;		
			vector<Sticker>			stickers;
			vector<BackgroundPrint> bgPrint;

			BackgroundPrint activeBgPrint;
			Sticker			activeSticker;

			bool hasDesignChanges(PhotoboothDataStruct val)
			{
				return (val.isCustomDesign != isCustomDesign ||
					val.templateId	   != templateId);
			}

			bool hasAnyChanges(PhotoboothDataStruct val)
			{
				return (val.sharing.getSocialState(SocialID::EMAIL)	 != sharing.getSocialState(SocialID::EMAIL) ||
					val.sharing.getSocialState(SocialID::FACEBOOK)	 != sharing.getSocialState(SocialID::FACEBOOK) ||
					val.sharing.getSocialState(SocialID::PRINTER)	 != sharing.getSocialState(SocialID::PRINTER)||
					val.sharing.getSocialState(SocialID::QRCODE)	 != sharing.getSocialState(SocialID::QRCODE)||					
					val.sharing.getSocialState(SocialID::TWITTER)	 != sharing.getSocialState(SocialID::TWITTER)||
					val.sharing.getSocialState(SocialID::VKONTAKTE)  != sharing.getSocialState(SocialID::VKONTAKTE)||
					val.isSticker			      != isSticker ||
					val.photoNum				  != photoNum ||
					val.seconds					  != seconds ||
					val.secondsBetweenShots		  != secondsBetweenShots ||
					val.activeSticker.id		  != activeSticker.id ||					
					val.activeBgPrint.id		  != activeBgPrint.id);
			}

			std::map <string, ConfigTitles> mainTitles, subTitles;
			
			void setMainTitles(string lang, ConfigTitles txts)
			{
				mainTitles[lang] = txts;			
			}

			void setSubTitles(string lang, ConfigTitles txts)
			{
				subTitles[lang] = txts;			
			}

			void setText(std::pair<TextID, string> key, string txts)
			{
				texts[key].set(txts);
			}

			string getText(std::pair<TextID, string> key)
			{
				return texts[key].get();
			}
			std::map <std::pair<TextID, string>, StringVO> texts;
		};

		string getMainTitle(SettingsPartID id)
		{
			return getData().mainTitles[model->getLang()].get(id);
		}

		string getSubTitle(SettingsPartID id)
		{
			return getData().mainTitles[model->getLang()].get(id);
		}		

		string getText(TextID id)
		{
			return getData().getText(make_pair(id, model->getLang()));
		}

		string getSocialTitle(SocialID id)
		{
			return getData().sharing.getSocialTitle(make_pair(id, model->getLang()));
		}

		bool getSocialState(SocialID id)
		{
			return getData().sharing.getSocialState(id);
		}

		Texture getIcon(SocialID id)
		{
			return getData().sharing.getIcon(id);
		}

		Texture getEmptyIcon()
		{
			return getData().sharing.getEmptyIcon();
		}

		PhotoboothSettings(ApplicationModelRef model);
		void load() override;
		void setTextures() override;

		Texture getActiveStickerTex();
		Texture getActivePrintBgTex();

		bool findFilterId(int id, vector<int> filters);		

		vector<int> getOnFilters();
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

		void findAllImagePrints(string path, vector<ImageElement> &prints, bool isCustom);
	};

	typedef shared_ptr<PhotoboothSettings> PhotoboothSettingsRef;
}