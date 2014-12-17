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

		struct PhotoboothDataStruct
		{
			static const int MIN_PHOTO_SHOTS = 1;
			static const int MAX_PHOTO_SHOTS = 3;

			static const int MIN_SEC_BETWEEN_SHOTS = 1;
			static const int MAX_SEC_BETWEEN_SHOTS = 3;

			static const int MIN_COUNTDOWN_TIMER = 3;
			static const int MAX_COUNTDOWN_TIMER = 5;			

			bool isFacebook;
			bool isVkotakte;
			bool isTwitter;
			bool isCustomDesign;
			bool isPrint;
			bool isEmail;
			bool isQrCode;
			bool isSticker;

			int  seconds;
			int  secondsBetweenShots;
			int  photoNum;
			int  templateId;
		
			vector<Filter> filters;		
			vector<Sticker> customStickers;
			vector<Sticker> kubikStickers;
			vector<BackgroundPrint> customBgPrint;
			vector<BackgroundPrint> kubikBgPrint;

			BackgroundPrint activeBgPrint;
			Sticker activeSticker;
		};		

		PhotoboothSettings(shared_ptr<ApplicationModel> model) 
		{
			this->model = model;
			mainConfigPath  = model->getPhotoboothConfigPath();
		}

		void load() override
		{
			setConfigPaths();
			setParams();
			setDesignPath();
			setTextures();
		}	

		void setConfigPaths()
		{
			JsonTree configJSON = JsonTree(loadFile(mainConfigPath + CONFIG_FILE));
			configPaths.staticPartDesignPath			= configJSON.getChild("staticPartDesignPath").getValue<string>();//"data\\interface\\"
			configPaths.kubikTemplatePartDesignPath		= configJSON.getChild("kubikTemplatePartDesignPath").getValue<string>();//"kubik\\templates\\"
			configPaths.userTemplatePartDesignPath		= configJSON.getChild("userTemplatePartDesignPath").getValue<string>();//"user_design\\templates\\"
			configPaths.finalPath						= configJSON.getChild("finalPath").getValue<string>();//"gameDesign\\photobooth\\"
			configPaths.kubikStickersPath				= configJSON.getChild("kubikStickersPath").getValue<string>();
			configPaths.userStickersPath				= configJSON.getChild("userStickersPath").getValue<string>();
			configPaths.userBgPrintsPath				= configJSON.getChild("userBgPrintsPath").getValue<string>();
			configPaths.kubikBgPrintsPath				= configJSON.getChild("kubikBgPrintsPath").getValue<string>();
		}

		void setParams()
		{
			JsonTree configJSON = JsonTree(loadFile(mainConfigPath + PARAMS_FILE));
			
			setPhotoParams(configJSON);
			setSocialParams(configJSON);
			setPhotoFilterParams(configJSON);
			setGameDesignParams(configJSON);
			setGameStickerParams(configJSON);
			setGameBgPrintParams(configJSON);	
		}

		void setPhotoParams(JsonTree config)
		{
			data.seconds				= config.getChild("seconds").getValue<int>();
			data.secondsBetweenShots	= config.getChild("secondsBetweenShots").getValue<int>();
			data.photoNum				= config.getChild("photoNum").getValue<int>();
		}

		void setSocialParams(JsonTree config)
		{
			data.isFacebook				= config.getChild("isFacebook").getValue<bool>();
			data.isVkotakte				= config.getChild("isVkotakte").getValue<bool>();
			data.isTwitter				= config.getChild("isTwitter").getValue<bool>();
			data.isEmail				= config.getChild("isEmail").getValue<bool>();
			data.isQrCode				= config.getChild("isQrCode").getValue<bool>();
			data.isPrint			    = config.getChild("isPrint").getValue<bool>();
		}

		void setPhotoFilterParams(JsonTree config)
		{
			JsonTree datas = JsonTree(config.getChild( "filtersIds"));
			JsonTree useIds	= JsonTree(config.getChild( "useIds"));

			vector<int> useIdsVec;

			for(auto it : useIds)
				useIdsVec.push_back(it.getChild("id").getValue<int>());

			for( auto it : datas)
			{
				Filter filter;
				filter.id	  = it.getChild("id").getValue<int>();
				filter.isOn = findFilterId(filter.id, useIdsVec);

				data.filters.push_back(filter);
			}
		}		

		void setGameDesignParams(JsonTree config)
		{
			data.templateId						= config.getChild("templateId").getValue<int>();		
			data.isCustomDesign					= config.getChild("isCustomDesign").getValue<bool>();	
		}

		void setGameStickerParams(JsonTree config)
		{
			data.isSticker						= config.getChild("isSticker").getValue<bool>();			
			data.activeSticker.isCustom			= config.getChild("activeSticker").getChild("isCustom").getValue<bool>();
			data.activeSticker.id				= config.getChild("activeSticker").getChild("id").getValue<int>();			
			findAllImagePrints(getBasePath().string() + configPaths.userStickersPath,  data.customStickers, true);
			findAllImagePrints(getBasePath().string() + configPaths.kubikStickersPath, data.kubikStickers,  false);
		}

		void setGameBgPrintParams(JsonTree config)
		{
			data.activeBgPrint.isCustom			= config.getChild("activeBgPrint").getChild("isCustom").getValue<bool>();
			data.activeBgPrint.id				= config.getChild("activeBgPrint").getChild("id").getValue<int>();

			findAllImagePrints(getBasePath().string() + configPaths.userBgPrintsPath,  data.customBgPrint, true);		
			findAllImagePrints(getBasePath().string() + configPaths.kubikBgPrintsPath, data.kubikBgPrint,  false);	
		}	

		void findAllImagePrints(string path, vector<ImageElement> &prints, bool isCustom) 
		{
			for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
			{
				if (fs::is_regular_file(*it))
				{
					string ext = it->path().extension().string();

					if(ext == STICKER_SUPPORT_EXTENSION)
					{
						BackgroundPrint print;
						print.isCustom = isCustom;
						print.path =  path + it->path().filename().string();
						print.id = prints.size();
						prints.push_back(print);
					}
				}
			}
		}

		Texture getActiveStickerTex()
		{
			Texture tex;
			string name;

			if(data.activeSticker.isCustom)			
				name = CUSTOM_STICKER_NAME + to_string(data.activeSticker.id);			
			else			
				name = KUBIK_STICKER_NAME + to_string(data.activeSticker.id);

			auto it = textures.find(name);

			if(it != textures.end())
				tex = textures[name]->get();			

			return tex;
		}

		bool findFilterId(int id, vector<int> filters)
		{
			for (auto it : filters)
			{
				if(it == id)
					return true;
			}
			return false;
		}

		void saveConfig()
		{
			fs::path basePath(mainConfigPath + PARAMS_FILE);

			JsonTree doc;

			doc.addChild( JsonTree("isFacebook", data.isFacebook));		
			doc.addChild( JsonTree("isVkotakte", data.isVkotakte));		
			doc.addChild( JsonTree("isTwitter", data.isTwitter));		
			doc.addChild( JsonTree("isEmail", data.isEmail));		
			doc.addChild( JsonTree("isQrCode", data.isQrCode));		
			doc.addChild( JsonTree("isPrint", data.isPrint));

			doc.addChild( JsonTree("photoNum", data.photoNum));		
			doc.addChild( JsonTree("seconds", data.seconds));		
			doc.addChild( JsonTree("secondsBetweenShots", data.secondsBetweenShots));

			doc.addChild( JsonTree("templateId", data.templateId));
			doc.addChild( JsonTree("isCustomDesign", data.isCustomDesign));
			doc.addChild( JsonTree("isSticker", data.isSticker));
		
			JsonTree filtersIdsJ = JsonTree::makeArray("filtersIds");
			JsonTree useIdsJ	 = JsonTree::makeArray("useIds");

			for (auto it: data.filters)
			{
				JsonTree id;
				id.addChild(JsonTree("id", it.id));
				filtersIdsJ.pushBack( id);

				if(it.isOn)
				{
					JsonTree id1;
					id1.addChild(JsonTree("id", it.id));
					useIdsJ.pushBack( id1);
				}
			}	

			JsonTree sticker = JsonTree::makeObject("activeSticker");
			sticker.addChild( JsonTree("isCustom", data.activeSticker.isCustom));
			sticker.addChild( JsonTree("id", data.activeSticker.id));
			doc.addChild(sticker);		

			JsonTree bgPrint = JsonTree::makeObject("activeBgPrint");
			bgPrint.addChild( JsonTree("isCustom", data.activeBgPrint.isCustom));
			bgPrint.addChild( JsonTree("id", data.activeBgPrint.id));
			doc.addChild(bgPrint);	

			doc.addChild(filtersIdsJ);
			doc.addChild(useIdsJ);
			doc.write( writeFile(basePath), JsonTree::WriteOptions() );
		}

		void setDesignPath()
		{
			if(data.isCustomDesign)
				templateDesignPath = configPaths.userTemplatePartDesignPath + to_string(data.templateId)+"\\" + configPaths.finalPath;
			else
				templateDesignPath = configPaths.kubikTemplatePartDesignPath + to_string(data.templateId)+"\\" + configPaths.finalPath;

			staticDesignPath = configPaths.staticPartDesignPath + configPaths.finalPath;
		}

		void setTextures() override
		{		
			clearResources();

			addToDictionary("closeImg",		createImageResource(getStaticDesignPath("close.png")));
			addToDictionary("fon1",			createImageResource(getTemplateDesignPath("PhotoInstruction\\1.jpg")));
			addToDictionary("fon2",			createImageResource(getTemplateDesignPath("PhotoFilter\\1.jpg")));
			addToDictionary("fon3",			createImageResource(getTemplateDesignPath("PhotoTimer\\1.jpg")));
			addToDictionary("helvetica40",  createFontResource(getFontsPath("Helvetica Neue.ttf"), 30));
			addToDictionary("helvetica100", createFontResource(getFontsPath("Helvetica Neue.ttf"), 100));

			for (int i = 0; i < data.customStickers.size(); i++)
				addToDictionary(CUSTOM_STICKER_NAME + to_string(i), createImageResource(data.customStickers[i].path));	

			for (int i = 0; i < data.kubikStickers.size(); i++)					
				addToDictionary(KUBIK_STICKER_NAME + to_string(i), createImageResource(data.kubikStickers[i].path));	
		}

		vector<int> getOnFilters()
		{
			vector<int> onFilters;

			for (size_t i = 0; i < data.filters.size(); i++)			
				if(data.filters[i].isOn)				
					onFilters.push_back(data.filters[i].id);				

			return onFilters;
		}

		int getPhotoShots()
		{
			return data.photoNum + 2;
		}

		PhotoboothDataStruct getData()
		{
			return data;
		}

		void setData(PhotoboothDataStruct value)
		{
			data = value;
			saveConfig();
		}

	private:
		PhotoboothDataStruct data;
		ConfigPath configPaths;
	};
}