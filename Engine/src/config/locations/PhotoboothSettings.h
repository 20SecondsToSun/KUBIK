#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"
#include "KubikException.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	static const string STICKER_SUPPORT_EXTENSION = ".png";
	static const string CONFIG_FILE = "path.txt";
	static const string PARAMS_FILE = "params.txt";

	static const string CUSTOM_STICKER_NAME = "custom_sticker";
	static const string KUBIK_STICKER_NAME  = "kubik_sticker";

	static const string CUSTOM_PRINT_TEMPATE_NAME = "custom_print_template";
	static const string KUBIK_PRINT_TEMPATE_NAME  = "kubik_print_template";

	class PhotoboothSettings:public ISettings
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
			bool isCustom;
		};

		typedef ImageElement Sticker;
		typedef ImageElement BackgroundPrint;

		struct ConfigPath
		{
			string staticPartDesignPath;
			string kubikTemplatePartDesignPath;
			string userTemplatePartDesignPath;
			string finalPath;

			string kubikStickersPath;
			string userStickersPath;

			string userBgPrintsPath;
			string kubikBgPrintsPath;
		};

		struct SharingStruct
		{
			bool isFacebook;
			bool isVkotakte;
			bool isTwitter;
			bool isPrint;
			bool isEmail;
			bool isQrCode;
		};

		struct PhotoboothDataStruct
		{
			static const int MIN_PHOTO_SHOTS = 1;
			static const int MAX_PHOTO_SHOTS = 3;

			static const int MIN_SEC_BETWEEN_SHOTS = 1;
			static const int MAX_SEC_BETWEEN_SHOTS = 6;

			static const int MIN_COUNTDOWN_TIMER = 3;
			static const int MAX_COUNTDOWN_TIMER = 5;

			SharingStruct sharing;
			
			int  seconds;
			int  secondsBetweenShots;
			int  photoNum;
			int  templateId;
			bool isCustomDesign;
			bool isSticker;

			vector<Filter>			filters;		
			vector<Sticker>			customStickers;
			vector<Sticker>			kubikStickers;
			vector<BackgroundPrint> customBgPrint;
			vector<BackgroundPrint> kubikBgPrint;

			BackgroundPrint activeBgPrint;
			Sticker			activeSticker;

			bool hasDesignChanges(PhotoboothDataStruct val)
			{
				return (val.isCustomDesign != isCustomDesign ||
						val.templateId	   != templateId);
			}

			bool hasAnyChanges(PhotoboothDataStruct val)
			{
				return (val.sharing.isEmail			  != sharing.isEmail ||
						val.sharing.isFacebook        != sharing.isFacebook ||
						val.sharing.isPrint			  != sharing.isPrint ||
						val.sharing.isQrCode		  != sharing.isQrCode ||
						val.isSticker			      != isSticker ||
						val.sharing.isTwitter	      != sharing.isTwitter ||
						val.sharing.isVkotakte	      != sharing.isVkotakte ||
						val.photoNum				  != photoNum ||
						val.seconds					  != seconds ||
						val.secondsBetweenShots		  != secondsBetweenShots ||
						val.activeSticker.isCustom	  != activeSticker.isCustom ||
						val.activeSticker.id		  != activeSticker.id ||
						val.activeBgPrint.isCustom	  != activeBgPrint.isCustom ||
						val.activeBgPrint.id		  != activeBgPrint.id);
			}
		};

		PhotoboothSettings(shared_ptr<ApplicationModel> model);
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
		void saveConfig();

		void findAllImagePrints(string path, vector<ImageElement> &prints, bool isCustom);
	};

	typedef shared_ptr<PhotoboothSettings> PhotoboothSettingsRef;
}