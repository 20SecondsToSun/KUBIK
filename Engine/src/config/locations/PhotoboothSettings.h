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

		void load(string path)
		{
			configPath = path;
			JsonTree configJSON = JsonTree(loadFile(path));

			seconds				= configJSON.getChild("seconds").getValue<int>();
			secondsBetweenShots = configJSON.getChild("secondsBetweenShots").getValue<int>();
			photoNum			= configJSON.getChild("photoNum").getValue<int>();
			isFacebook			= configJSON.getChild("isFacebook").getValue<bool>();
			isVkotakte			= configJSON.getChild("isVkotakte").getValue<bool>();
			isTwitter			= configJSON.getChild("isTwitter").getValue<bool>();
			isEmail				= configJSON.getChild("isEmail").getValue<bool>();
			isQrCode			= configJSON.getChild("isQrCode").getValue<bool>();
			isPrint			    = configJSON.getChild("isPrint").getValue<bool>();

			JsonTree datas = JsonTree(configJSON.getChild( "filtersIds"));
			
			for( auto data : datas)		
				filtersIds.push_back(data.getChild("id").getValue<int>());

			staticPartDesignPath			= configJSON.getChild("staticPartDesignPath").getValue<string>();//"data\\interface\\"
			kubikTemplatePartDesignPath		= configJSON.getChild("kubikTemplatePartDesignPath").getValue<string>();//"kubik\\templates\\"
			userTemplatePartDesignPath		= configJSON.getChild("userTemplatePartDesignPath").getValue<string>();//"user_design\\templates\\"
			finalPath						= configJSON.getChild("finalPath").getValue<string>();//"gameDesign\\photobooth\\"
			templateId						= configJSON.getChild("templateId").getValue<int>();

			isCustomDesign					= configJSON.getChild("isCustomDesign").getValue<bool>();

			isSticker						= configJSON.getChild("templateId").getValue<bool>();
			stickerID						= configJSON.getChild("stickerID").getValue<int>();
			frameMode						= configJSON.getChild("frameMode").getValue<int>();

			setDesignPath();		
		
			setTextures();
		}

		void saveConfig()
		{
			fs::path basePath(configPath);

			JsonTree doc;

			doc.addChild( JsonTree("isFacebook", isFacebook));		
			doc.addChild( JsonTree("isVkotakte", isVkotakte));		
			doc.addChild( JsonTree("isTwitter", isTwitter));		
			doc.addChild( JsonTree("isEmail", isEmail));		
			doc.addChild( JsonTree("isQrCode", isQrCode));		
			doc.addChild( JsonTree("isPrint", isPrint));

			doc.addChild( JsonTree("photoNum", photoNum));		
			doc.addChild( JsonTree("seconds", seconds));		
			doc.addChild( JsonTree("secondsBetweenShots", secondsBetweenShots));


			doc.addChild( JsonTree("staticPartDesignPath", staticPartDesignPath));
			doc.addChild( JsonTree("kubikTemplatePartDesignPath", kubikTemplatePartDesignPath));
			doc.addChild( JsonTree("userTemplatePartDesignPath", userTemplatePartDesignPath));
			doc.addChild( JsonTree("finalPath", finalPath));
			doc.addChild( JsonTree("templateId", templateId));
			doc.addChild( JsonTree("isCustomDesign", isCustomDesign));
			doc.addChild( JsonTree("isSticker", isSticker));
			doc.addChild( JsonTree("stickerID", stickerID));
			doc.addChild( JsonTree("frameMode", frameMode));

			JsonTree filtersIdsJ = JsonTree::makeArray("filtersIds");

			for (auto it: filtersIds)
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
			if(isCustomDesign)
				templateDesignPath = userTemplatePartDesignPath + to_string(templateId)+"\\" + finalPath;
			else
				templateDesignPath = kubikTemplatePartDesignPath + to_string(templateId)+"\\" + finalPath;

			staticDesignPath = staticPartDesignPath+ finalPath;
		}

		void setTextures()
		{		
			designTexures.clear();
			addToDictionary("closeImg", getTemplateDesignPath() + "close.png");
		}

		void setSeconds(int _seconds)
		{
			seconds = _seconds;
		}

		int getSeconds()
		{
			return seconds;
		}

		void setSecondsBetweenShots(int _secondsBetweenShots)
		{
			secondsBetweenShots = _secondsBetweenShots;
		}

		int getSecondsBetweenShots( )
		{
			return secondsBetweenShots ;
		}	

		bool getFacebook()
		{
			return isFacebook;
		}

		bool getVontakte()
		{
			return isVkotakte;
		}

		bool getTwitter()
		{
			return isTwitter;
		}

		bool getEmail()
		{
			return isEmail;
		}

		bool getQrCode()
		{
			return isQrCode;
		}

		bool getPrint()
		{
			return isPrint;
		}	

		int getPhotoNum( )
		{
			return photoNum ;
		}

		void setFacebook(bool value)
		{
			isFacebook = value;
		}

		void setVontakte(bool value)
		{
			isVkotakte = value;
		}

		void setPhotoNum(int value)
		{
			photoNum = value;
		}

	private:
		string configPath;

		vector<int> filtersIds;

		int secondsBetweenShots;
		int seconds;
		int photoNum;

		bool isFacebook, isVkotakte, isTwitter, isEmail, isQrCode, isPrint;

		string staticPartDesignPath;	
		string	kubikTemplatePartDesignPath	;
		string userTemplatePartDesignPath;
		string finalPath;
		int templateId;
		bool isCustomDesign;
		bool isSticker;
		int stickerID;
		int frameMode;
	};
}