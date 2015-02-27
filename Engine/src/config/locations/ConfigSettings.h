#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "KubikException.h"
#include "DesignData.h"
#include "JsonTools.h"
#include "ConfigTexts.h"
#include "StringTools.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{	
	namespace config
	{
	class ConfigSettings:public ISettings
	{
	public:
		static enum TextID
			{
				SWITCH_OFF,
				CONFIG,
				STAT,
				NOTINSTALL,
				DESIGNMAIN,
				DESIGNSUB,
				FINANDSTART,
				PLAYED_COUNT,
				PRINTED_COUNT, 
				SOCIAL_COUNT,
				LOADMAIN,
				LOADSUB,
				LOAD,
				BACK,

				CATRINGE_CHANGED,
				PHOTO_LEFT,
				PRINTER_ASK_TITLE,
				PRINTER_ASK_SUB_TITLE,
				PRINTER_CLOSE_TITLE,
				PRINTER_OK_TITLE,

				PARTY_ASK_TITLE,
				PARTY_DESQR,
				PARTY_TITLE,
				PARTY_BEGIN,
				PARTY_CLOSE
			};

		struct ConfighDataStruct
		{
			std::string actionName;
			int playedCount;
			int printedCount;
			int puplishedCount;
			int currentPhotosPrinted;
			int maxPhotosToPrint;
			int activeDesign;
			int userDesignID;
			ci::Color activeDesignColor;	
			
			/////////////////////////////////			
					
			std::string lang;
			void setLang(const std::string& lang)
			{
				this->lang = lang;
			}

			/////////////////////////////////

			ConfigTexts<TextID> configTexts;
			ConfigTexts<TextID> getTexts()
			{
				return configTexts;
			}

			void setTexts(ConfigTexts<TextID> val)
			{
				configTexts = val;
			}	

			/////////////////////////////////

			DesignData designData;
			DesignData getDesignData()
			{
				return designData;
			}

		}data;

	public:
		ConfigSettings(ApplicationModelRef model):ISettings(model)
		{
			mainConfigPath = model->getTuneUpConfigPath();
			data.designData = model->getDesignData();
			data.userDesignID = model->getUserDesignID();
		}

		void load() override
		{
			logger().log("config settings load " + mainConfigPath);
			try	
			{
				JsonTree configJSON		  = JsonTree(loadFile(mainConfigPath));
				designPath				  = configJSON.getChild("designPath").getValue<string>();
				data.actionName			  = configJSON.getChild("actionName").getValue<string>();
				data.playedCount		  = configJSON.getChild("playedCount").getValue<int>();
				data.printedCount		  = configJSON.getChild("printedCount").getValue<int>();
				data.puplishedCount		  = configJSON.getChild("puplishedCount").getValue<int>();
				data.currentPhotosPrinted = configJSON.getChild("currentPhotosPrinted").getValue<int>();
				data.maxPhotosToPrint	  = configJSON.getChild("maxPhotosToPrint").getValue<int>();

				data.activeDesign		  = configJSON.getChild("activeDesign").getValue<int>();
				data.activeDesignColor	  = stringTools().stringToColor(configJSON.getChild("activeColor").getValue<string>());
				
				data.setLang(this->model->getLang());
				
				JsonTree labelJSON  = JsonTree(loadFile(model->getLabelsPath()));	

				ConfigTexts<TextID> txts;

				JsonTree jsonTexts   = JsonTree(labelJSON.getChild("apps"));
				for(auto it : jsonTexts)
				{					
					string lang	= it.getChild("lang").getValue<string>();	
					txts.insert(lang, TextID::SWITCH_OFF,	jtools().parseTextItem(it.getChild("switchoff")));
					txts.insert(lang, TextID::CONFIG,		jtools().parseTextItem(it.getChild("config")));
					txts.insert(lang, TextID::STAT,			jtools().parseTextItem(it.getChild("stat")));
					txts.insert(lang, TextID::NOTINSTALL,	jtools().parseTextItem(it.getChild("notinstall")));				
				}
			
				jsonTexts  = JsonTree(labelJSON.getChild("design"));
				for(auto it : jsonTexts)
				{
					string lang	= it.getChild("lang").getValue<string>();	
					txts.insert(lang, TextID::DESIGNMAIN,	jtools().parseTextItem(it.getChild("designMain")));
					txts.insert(lang, TextID::DESIGNSUB,	jtools().parseTextItem(it.getChild("designSub")));					
				}

				jsonTexts  = JsonTree(labelJSON.getChild("stat"));
				for(auto it : jsonTexts)
				{
					string lang	= it.getChild("lang").getValue<string>();	
					txts.insert(lang, TextID::FINANDSTART,	jtools().parseTextItem(it.getChild("finAndStart")));
					txts.insert(lang, TextID::PLAYED_COUNT,	jtools().parseTextItem(it.getChild("playedCount")));
					txts.insert(lang, TextID::PRINTED_COUNT,   jtools().parseTextItem(it.getChild("printedCount")));
					txts.insert(lang, TextID::SOCIAL_COUNT,	jtools().parseTextItem(it.getChild("socialCount")));						
				}
			
				jsonTexts  = JsonTree(labelJSON.getChild("screensaver"));
				for(auto it : jsonTexts)
				{
					string lang	= it.getChild("lang").getValue<string>();	
					txts.insert(lang, TextID::LOADMAIN,	 jtools().parseTextItem(it.getChild("loadMain")));
					txts.insert(lang, TextID::LOADSUB, jtools().parseTextItem(it.getChild("loadSub")));					
				}

				jsonTexts  = JsonTree(labelJSON.getChild("screensaver_back"));
				for(auto it : jsonTexts)
				{
					string lang	= it.getChild("lang").getValue<string>();	
					txts.insert(lang, TextID::BACK, jtools().parseTextItem(it.getChild("back")));
				}

				jsonTexts = JsonTree(labelJSON.getChild("load"));
				for(auto it : jsonTexts)
				{
					string lang	= it.getChild("lang").getValue<string>();	
					txts.insert(lang, TextID::LOAD, jtools().parseTextItem(it.getChild("load")));
				}
				
				jsonTexts = JsonTree(labelJSON.getChild("printer"));
				for(auto it : jsonTexts)
				{
					string lang	     = it.getChild("lang").getValue<string>();										
					txts.insert(lang, TextID::PHOTO_LEFT, jtools().parseTextItem(it.getChild("catridgeChange")));
					txts.insert(lang, TextID::CATRINGE_CHANGED, jtools().parseTextItem(it.getChild("changed")));
					txts.insert(lang, TextID::PRINTER_ASK_TITLE, jtools().parseTextItem(it.getChild("changeTitle")));
					txts.insert(lang, TextID::PRINTER_ASK_SUB_TITLE, jtools().parseTextItem(it.getChild("changeSubTitle")));
					txts.insert(lang, TextID::PRINTER_CLOSE_TITLE, jtools().parseTextItem(it.getChild("closeTitle")));
					txts.insert(lang, TextID::PRINTER_OK_TITLE, jtools().parseTextItem(it.getChild("okTitle")));
				}
			
				jsonTexts = JsonTree(labelJSON.getChild("newParty"));
				for(auto it : jsonTexts)
				{
					string lang	     = it.getChild("lang").getValue<string>();										
					txts.insert(lang, TextID::PARTY_ASK_TITLE, jtools().parseTextItem(it.getChild("askTitle")));
					txts.insert(lang, TextID::PARTY_DESQR, jtools().parseTextItem(it.getChild("desqr")));
					txts.insert(lang, TextID::PARTY_TITLE, jtools().parseTextItem(it.getChild("title")));
					txts.insert(lang, TextID::PARTY_BEGIN, jtools().parseTextItem(it.getChild("begin")));
					txts.insert(lang, TextID::PARTY_CLOSE, jtools().parseTextItem(it.getChild("close")));		
				}
			
				data.setTexts(txts);
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
			addToDictionary("helveticaLight20",  createFontResource(getFontsPath("HelveticaLight.ttf"), 20));
			addToDictionary("helvetica90",		 createFontResource(getFontsPath("Helvetica Neue.ttf"), 90));
			addToDictionary("helvetica20",		 createFontResource(getFontsPath("Helvetica Neue.ttf"), 20));
			addToDictionary("helveticaLight24",  createFontResource(getFontsPath("HelveticaLight.ttf"), 24));
			addToDictionary("helveticaLight36",  createFontResource(getFontsPath("HelveticaLight.ttf"), 36));

			addToDictionary("introBold110",		 createFontResource(getFontsPath("introb.ttf"), 110));
			addToDictionary("introBold72",		 createFontResource(getFontsPath("introb.ttf"), 72));
			addToDictionary("introLight44",		 createFontResource(getFontsPath("IntroLight.ttf"), 44));
			addToDictionary("introLight36",		 createFontResource(getFontsPath("IntroLight.ttf"), 36));

			addToDictionary("introLight72",		 createFontResource(getFontsPath("IntroLight.ttf"), 72));
			addToDictionary("introLight30",		 createFontResource(getFontsPath("IntroLight.ttf"), 30));			

			addToDictionary("iconClose",		 createImageResource(getDesignPath() + "closeConfig.png"));
			addToDictionary("iconBack",			 createImageResource(getDesignPath() + "backIcon.png"));

			addToDictionary("tempBg",			 createImageResource(getDesignPath() + "tempMain.jpg"));
			addToDictionary("iconStartNew",		 createImageResource(getDesignPath() + "iconStartNew.png"));
			addToDictionary("designIcon",		 createImageResource(getDesignPath() + "ssIcon.png"));
			//addToDictionary("menuIcon",		 createImageResource(getDesignPath() + "menuIcon.png"));

			addToDictionary("tempBottom",		 createImageResource(getDesignPath() + "tempBottom.png"));
			addToDictionary("catridgeIcon",		 createImageResource(getDesignPath() + "catridgeIcon.png"));
			addToDictionary("catridgeIcon",		 createImageResource(getDesignPath() + "catridgeIcon.png"));
			addToDictionary("logoIcon",			 createImageResource(getDesignPath() + "logo.png"));
			addToDictionary("circles",			 createImageResource(getDesignPath() + "circles.png"));
			addToDictionary("loadIcon",			 createImageResource(getDesignPath() + "loadIcon.png"));
			addToDictionary("closePrinterIcon",	 createImageResource(getDesignPath() + "main/closePrinterIcon.png"));
			addToDictionary("inputField",		 createImageResource(getDesignPath() + "main/inputField.png"));
			
			

			addToDictionary("ssCheckerOn",		 createImageResource(getDesignPath() + "main/ssCheckerOn.png"));
			addToDictionary("ssCheckerOff",		 createImageResource(getDesignPath() + "main/ssCheckerOff.png"));

			for (auto item : data.designData)			
				addToDictionary(item.getName(),	createImageResource(getDesignPath() + "main/" + item.getIconPath()));
						
			addToDictionary("appsTemp",			createImageResource(getDesignPath() + "appsTemp.png"));
			addToDictionary("gameInWebIcon",	createImageResource(getDesignPath() + "gameInWebIcon.png"));
			addToDictionary("temp",				createImageResource(getDesignPath() + "temp.png"));
			addToDictionary("temp1",			createImageResource(getDesignPath() + "temp1.png"));
			addToDictionary("temp2",			createImageResource(getDesignPath() + "temp2.png"));
			addToDictionary("temp3",			createImageResource(getDesignPath() + "temp3.png"));
			addToDictionary("partyTemp",		createImageResource(getDesignPath() + "PartyTemp.png"));			
		}

		void buildData()
		{
			logger().log("main config build");
			auto texts = data.getTexts();
			auto dic = texts.getDic();
			
			logger().log(to_string(dic.size()));	
			 for (ConfigTexts<TextID>::DicType::iterator it = dic.begin(); it != dic.end(); ++it)			 
				it->second.setFont(fonts);				 
			
			texts.setDic(dic);		
			data.setTexts(texts);
		}

		int  setActiveDesignID(int id)	
		{
			data.activeDesign = id;
		}

		int getUserDesignID()
		{
			return data.userDesignID;
		}

		int  getActiveDesignID()	
		{
			return data.activeDesign;
		}

		ci::Color  getActiveDesignColor()	
		{
			return data.activeDesignColor;
		}
	
		TextItem  getTextItem(TextID id)	
		{
			return data.getTexts().get(model->getLang(), id);
		}	

		std::string getActionName() const
		{
			return data.actionName;
		}

		DesignData getDesignData()
		{
			return data.getDesignData();
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

	typedef ConfigSettings::TextID ConfigTextID;
	typedef shared_ptr<ConfigSettings> ConfigSettingsRef;	
	}
}