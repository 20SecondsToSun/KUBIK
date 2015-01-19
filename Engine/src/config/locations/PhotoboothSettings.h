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

	static const string STICKER_NAME = "sticker";
	static const string PRINT_TEMPATE_NAME = "print_template";

	class PhotoboothSettings: public ISettings
	{
	public:

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

		class Sharing
		{
		public:
			bool getFacebook(){return facebookOn;}
			bool getVkontakte(){return vkotakteOn;}
			bool getTwitter(){return twitterOn;}
			bool getPrint(){return printOn;}
			bool getEmail(){return emailOn;}
			bool getQrCode(){return qrCodeOn;}	

			void setFacebook(bool value){facebookOn = value;}
			void setVkontakte(bool value){vkotakteOn = value;}
			void setTwitter(bool value){twitterOn = value;}
			void setPrint(bool value){printOn = value;}
			void setEmail(bool value){emailOn = value;}
			void setQrCode(bool value){qrCodeOn = value;}

			friend PhotoboothSettings;
		private:
			bool facebookOn;
			bool vkotakteOn;
			bool twitterOn;
			bool printOn;
			bool emailOn;
			bool qrCodeOn;			
		};

		class ConfigTexts
		{
		public:
			string getDesignInterfaceText(){return designInterfaceText; }
			string getPhotoStyleText()	{return photoOverText;}
			string getPhotoPrintCountText(){return photoPrintCountText;}
			string getPhotoOverElementsText(){return photoOverElementsText;}
			string getPhotoFiltersText(){return photoFiltersTex;}
			string getPublishText(){return publishText;}	

			void setDesignInterfaceText(string value){designInterfaceText = value;}
			void setPhotoStyleText(string value)	{photoOverText = value;}
			void setPhotoPrintCountText(string value){photoPrintCountText = value;}
			void setPhotoOverElementsText(string value){photoOverElementsText = value;}
			void setPhotoFiltersText(string value){photoFiltersTex = value;}
			void setPublishText(string value){publishText = value;}				

		private:
			string designInterfaceText;
			string	photoOverText;
			string photoPrintCountText;
			string photoOverElementsText;
			string photoFiltersTex;
			string publishText;
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
				return (val.sharing.getEmail()	  != sharing.getEmail() ||
					val.sharing.getFacebook()     != sharing.getFacebook() ||
					val.sharing.getPrint()		  != sharing.getPrint() ||
					val.sharing.getQrCode()		  != sharing.getQrCode() ||
					val.isSticker			      != isSticker ||
					val.sharing.getTwitter()	  != sharing.getTwitter() ||
					val.sharing.getVkontakte()     != sharing.getVkontakte() ||
					val.photoNum				  != photoNum ||
					val.seconds					  != seconds ||
					val.secondsBetweenShots		  != secondsBetweenShots ||
					val.activeSticker.id		  != activeSticker.id ||					
					val.activeBgPrint.id		  != activeBgPrint.id);
			}

			std::map <string, ConfigTexts> mainTitles, subTitles;
			std::map <string, StringVO> yorDesignText, saveText;

			void setMainTitles(string lang, ConfigTexts txts)
			{
				mainTitles[lang] = txts;			
			}

			void setSubTitles(string lang, ConfigTexts txts)
			{
				subTitles[lang] = txts;			
			}

			void setYourDesignText(string lang, StringVO txts)
			{
				yorDesignText[lang] = txts;			
			}

			void setSaveText(string lang, StringVO txts)
			{
				saveText[lang] = txts;			
			}
		};

		ConfigTexts getMainTitles()
		{
			return getData().mainTitles[this->model->getLang()];
		}

		ConfigTexts getSubTitles()
		{
			return getData().subTitles[this->model->getLang()];
		}

		string getYourDesignText()
		{
			return getData().yorDesignText[this->model->getLang()].get();
		}

		string getSaveText()
		{
			return getData().saveText[this->model->getLang()].get();
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

		void saveConfig();

		void findAllImagePrints(string path, vector<ImageElement> &prints, bool isCustom);
	};

	typedef shared_ptr<PhotoboothSettings> PhotoboothSettingsRef;
}