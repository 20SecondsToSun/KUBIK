#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "KubikException.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class ConfigSettings:public ISettings
	{
		struct ConfighDataStruct
		{
			string actionName;
			int playedCount;
			int printedCount;
			int puplishedCount;
			int currentPhotosPrinted;
			int maxPhotosToPrint;
		}data;

	public:
		ConfigSettings(shared_ptr<ApplicationModel> model)
		{
			this->model = model;
			mainConfigPath = model->getTuneUpConfigPath();
		}

		void load() override
		{
			try	
			{
				JsonTree configJSON = JsonTree(loadFile(mainConfigPath));
				designPath = configJSON.getChild("designPath").getValue<string>();
				data.actionName = configJSON.getChild("actionName").getValue<string>();
				data.playedCount = configJSON.getChild("playedCount").getValue<int>();
				data.printedCount = configJSON.getChild("printedCount").getValue<int>();
				data.puplishedCount = configJSON.getChild("puplishedCount").getValue<int>();
				data.currentPhotosPrinted = configJSON.getChild("currentPhotosPrinted").getValue<int>();
				data.maxPhotosToPrint = configJSON.getChild("maxPhotosToPrint").getValue<int>();

				setTextures();
			}
			catch(...)
			{
				throw ExcConfigFileParsing();
			}
		}

		void setTextures()
		{
			addToDictionary("helveticaLight22",  createFontResource(getFontsPath("HelveticaLight.ttf"), 22));
			addToDictionary("helvetica90",  createFontResource(getFontsPath("Helvetica Neue.ttf"), 90));
			addToDictionary("helvetica20",  createFontResource(getFontsPath("Helvetica Neue.ttf"), 20));
			addToDictionary("helveticaLight24",  createFontResource(getFontsPath("Helvetica Neue.ttf"), 24));

			addToDictionary("introBold110",  createFontResource(getFontsPath("introb.ttf"), 110));
			addToDictionary("introBold72",  createFontResource(getFontsPath("introb.ttf"), 72));
			addToDictionary("introLight44",  createFontResource(getFontsPath("IntroLight.ttf"), 44));

			addToDictionary("closeImg",		 createImageResource(getDesignPath() + "closeConfig.png"));
			addToDictionary("tempBg",		 createImageResource(getDesignPath() + "tempMain.jpg"));
			addToDictionary("iconStartNew",	 createImageResource(getDesignPath() + "iconStartNew.png"));
			addToDictionary("ssIcon",		 createImageResource(getDesignPath() + "ssIcon.png"));
			addToDictionary("menuIcon",		 createImageResource(getDesignPath() + "menuIcon.png"));

			addToDictionary("tempBottom",		 createImageResource(getDesignPath() + "tempBottom.png"));
			addToDictionary("catridgeIcon",		 createImageResource(getDesignPath() + "catridgeIcon.png"));
			addToDictionary("catridgeIcon",		 createImageResource(getDesignPath() + "catridgeIcon.png"));
			addToDictionary("logoIcon",			 createImageResource(getDesignPath() + "logo.png"));
		}	

		ConfighDataStruct getData()	
		{
			return data;
		}

		void setData(ConfighDataStruct value)
		{
			data = value;
			saveConfig();
		}	

		void saveConfig()
		{
			fs::path basePath(mainConfigPath);
			JsonTree doc;
		
			doc.addChild(JsonTree("designPath", designPath));		
			doc.addChild(JsonTree("actionName", data.actionName));		
			doc.addChild(JsonTree("playedCount",  data.playedCount));		
			doc.addChild(JsonTree("puplishedCount",    data.puplishedCount));		
			doc.addChild(JsonTree("printedCount",    data.printedCount));		
			doc.addChild(JsonTree("currentPhotosPrinted",	data.currentPhotosPrinted));		
			doc.addChild(JsonTree("maxPhotosToPrint",	data.maxPhotosToPrint));
		}

	};

	typedef shared_ptr<ConfigSettings> ConfigSettingsRef;	
}