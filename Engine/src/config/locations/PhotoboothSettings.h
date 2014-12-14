#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class PhotoboothSettings:public ISettings
	{
	public:	

		enum PhotoboothDesignTemplates
		{
			POP_ART,
			NATURE,
			ABSTRACT
		};

		struct PhotoboothDataStruct
		{
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
			int  stickerID;
			int  frameMode;

			string staticPartDesignPath;
			string kubikTemplatePartDesignPath;
			string userTemplatePartDesignPath;
			string finalPath;

			vector<int> filtersIds;
		};

		PhotoboothSettings(shared_ptr<ApplicationModel> model) 
		{
			this->model = model;
			configPath = model->getPhotoboothConfigPath();
		}

		void load() override
		{			
			JsonTree configJSON = JsonTree(loadFile(configPath));
		
			data.seconds				= configJSON.getChild("seconds").getValue<int>();
			data.secondsBetweenShots	= configJSON.getChild("secondsBetweenShots").getValue<int>();
			data.photoNum				= configJSON.getChild("photoNum").getValue<int>();
			data.isFacebook				= configJSON.getChild("isFacebook").getValue<bool>();
			data.isVkotakte				= configJSON.getChild("isVkotakte").getValue<bool>();
			data.isTwitter				= configJSON.getChild("isTwitter").getValue<bool>();
			data.isEmail				= configJSON.getChild("isEmail").getValue<bool>();
			data.isQrCode				= configJSON.getChild("isQrCode").getValue<bool>();
			data.isPrint			    = configJSON.getChild("isPrint").getValue<bool>();
		
			JsonTree datas = JsonTree(configJSON.getChild( "filtersIds"));
			
			for( auto it : datas)		
				data.filtersIds.push_back(it.getChild("id").getValue<int>());
		
			data.staticPartDesignPath			= configJSON.getChild("staticPartDesignPath").getValue<string>();//"data\\interface\\"
			data.kubikTemplatePartDesignPath	= configJSON.getChild("kubikTemplatePartDesignPath").getValue<string>();//"kubik\\templates\\"
			data.userTemplatePartDesignPath		= configJSON.getChild("userTemplatePartDesignPath").getValue<string>();//"user_design\\templates\\"
			data.finalPath						= configJSON.getChild("finalPath").getValue<string>();//"gameDesign\\photobooth\\"
			data.templateId						= configJSON.getChild("templateId").getValue<int>();
		
			data.isCustomDesign					= configJSON.getChild("isCustomDesign").getValue<bool>();
		
			data.isSticker						= configJSON.getChild("isSticker").getValue<bool>();
			data.stickerID						= configJSON.getChild("stickerID").getValue<int>();
			data.frameMode						= configJSON.getChild("frameMode").getValue<int>();

			setDesignPath();
			setTextures();
		}

		void saveConfig()
		{
			fs::path basePath(configPath);

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

			doc.addChild( JsonTree("staticPartDesignPath", data.staticPartDesignPath));
			doc.addChild( JsonTree("kubikTemplatePartDesignPath", data.kubikTemplatePartDesignPath));
			doc.addChild( JsonTree("userTemplatePartDesignPath", data.userTemplatePartDesignPath));
			doc.addChild( JsonTree("finalPath", data.finalPath));
			doc.addChild( JsonTree("templateId", data.templateId));
			doc.addChild( JsonTree("isCustomDesign", data.isCustomDesign));
			doc.addChild( JsonTree("isSticker", data.isSticker));
			doc.addChild( JsonTree("stickerID", data.stickerID));
			doc.addChild( JsonTree("frameMode", data.frameMode));

			JsonTree filtersIdsJ = JsonTree::makeArray("filtersIds");

			for (auto it: data.filtersIds)
			{
				JsonTree id;
				id.addChild(JsonTree("id", it));
				filtersIdsJ.pushBack( id);
			}
			
			doc.addChild(filtersIdsJ);
			doc.write( writeFile(basePath), JsonTree::WriteOptions() );
		}

		void setDesignPath()
		{
			if(data.isCustomDesign)
				templateDesignPath = data.userTemplatePartDesignPath + to_string(data.templateId)+"\\" + data.finalPath;
			else
				templateDesignPath = data.kubikTemplatePartDesignPath + to_string(data.templateId)+"\\" + data.finalPath;

			staticDesignPath = data.staticPartDesignPath + data.finalPath;
		}

		void setTextures() override
		{		
			clearResources();
		
			addToDictionary("closeImg",		createImageResource(getStaticDesignPath("close.png")));
			addToDictionary("fon1",			createImageResource(getTemplateDesignPath("PhotoInstruction\\1.jpg")));
			addToDictionary("fon2",			createImageResource(getTemplateDesignPath("PhotoFilter\\1.jpg")));
			addToDictionary("fon3",			createImageResource(getTemplateDesignPath("PhotoTimer\\1.jpg")));
			addToDictionary("helvetica40",  createFontResource(getFontsPath("Helvetica Neue.ttf"), 30));
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
	};
}