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
				int publishedCount;
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

			}data, mementoData;

			bool memento;

			ConfigObject mainConfigObj;

			std::string userDesignPath;

		public:
			ConfigSettings(ApplicationModelRef model):ISettings(model), memento(false)
			{				
				data.designData = model->getDesignData();
				data.userDesignID = model->getUserDesignID();
				data.setLang(model->getLang());
			}

			//////////////////////////////////////////////////////////////////

			void setCurrentPhotosPrinted(int value)
			{
				data.currentPhotosPrinted = value;
			}

			int getCurrentPhotosPrinted()
			{
				return data.currentPhotosPrinted;
			}

			//////////////////////////////////////////////////////////////////

			void setPlayedCount(int value)
			{
				data.playedCount = value;
			}

			int getPlayedCount()
			{
				return data.playedCount;
			}

			//////////////////////////////////////////////////////////////////

			void setPrintedCount(int value)
			{
				data.printedCount = value;
			}

			int getPrintedCount()
			{
				return data.printedCount;
			}

			//////////////////////////////////////////////////////////////////

			void setPuplishedCount(int value)
			{
				data.publishedCount = value;
			}

			int getPublishedCount()
			{
				return data.publishedCount;
			}

			//////////////////////////////////////////////////////////////////			

			int  getActiveDesignID() const	
			{
				return data.activeDesign;
			}

			void  setActiveDesignID(int id)	
			{
				data.activeDesign = id;
			}	

			//////////////////////////////////////////////////////////////////

			ci::Color getActiveDesignColor() const	
			{
				return data.activeDesignColor;
			}

			TextItem getTextItem(TextID id)	
			{
				return data.getTexts().get(model->getLang(), id);
			}	

			//////////////////////////////////////////////////////////////////

			std::string getActionName() const
			{
				return data.actionName;
			}

			void setActionName(const std::string& value) 
			{
				data.actionName = value;
			}

			DesignData getDesignData()
			{
				return data.getDesignData();
			}

			std::string getUserDesignPath()
			{
				return getBasePath().string() + userDesignPath;
			}

			/*std::string getScreenSaverPath()
			{
				return getBasePath().string() + screenSaverPath;
			}*/

			ConfighDataStruct getData()	const
			{
				return data;
			}

			int getUserDesignID() const
			{
				return data.userDesignID;
			}

			void createMemento()
			{
				memento = true;
				mementoData = data;
			}			

			bool gamesChanged()
			{
				return false;
			}	

			//////////////////////////////////////////////////////////////////

			void load() override
			{
				logger().log("config settings load ");
				mainConfigObj  = model->getConfigObject(settings::id::MAINCONFIG);

				loadPaths();
				loadConsts();
				loadParams();					
				loadLabels();	
				setTextures();
			}				

			void loadPaths()
			{
				try	
				{
					JsonTree pathJSON  = JsonTree(loadFile(mainConfigObj.getPathsConfigPath()));
					designPath		   = pathJSON.getChild("designPath").getValue<string>();
					userDesignPath	   = pathJSON.getChild("userDesignPath").getValue<string>();
					//screenSaverPath	   = pathJSON.getChild("screenSaverPath").getValue<string>();
				}
				catch(...)
				{
					throw ExcConfigFileParsing();
				}
			}

			void loadConsts()
			{
				try	
				{
					JsonTree constJSON		  = JsonTree(loadFile(mainConfigObj.getConstsConfigPath()));
					data.maxPhotosToPrint	  = constJSON.getChild("maxPhotosToPrint").getValue<int>();
					data.activeDesignColor	  = stringTools().stringToColor(constJSON.getChild("activeColor").getValue<string>());

				}
				catch(...)
				{
					throw ExcConfigFileParsing();
				}
			}

			void loadParams()
			{
				try	
				{
					JsonTree configJSON		  = JsonTree(loadFile(mainConfigObj.getParamsConfigPath()));
					data.actionName			  = configJSON.getChild("actionName").getValue<string>();
					data.playedCount		  = configJSON.getChild("playedCount").getValue<int>();
					data.printedCount		  = configJSON.getChild("printedCount").getValue<int>();
					data.publishedCount		  = configJSON.getChild("publishedCount").getValue<int>();
					data.currentPhotosPrinted = configJSON.getChild("currentPhotosPrinted").getValue<int>();
					data.activeDesign		  = configJSON.getChild("activeDesign").getValue<int>();
				}
				catch(...)
				{
					throw ExcConfigFileParsing();
				}
			}

			void loadLabels()
			{
				try	
				{
					JsonTree labelJSON  = JsonTree(loadFile(mainConfigObj.getLabelsConfigPath()));						
					JsonTree jsonTexts  = JsonTree(labelJSON.getChild("apps"));

					for(auto it : jsonTexts)
					{					
						string lang	= it.getChild("lang").getValue<string>();	
						data.configTexts.insert(lang, TextID::SWITCH_OFF,	jtools().parseTextItem(it.getChild("switchoff")));
						data.configTexts.insert(lang, TextID::CONFIG,		jtools().parseTextItem(it.getChild("config")));
						data.configTexts.insert(lang, TextID::STAT,			jtools().parseTextItem(it.getChild("stat")));
						data.configTexts.insert(lang, TextID::NOTINSTALL,	jtools().parseTextItem(it.getChild("notinstall")));				
					}

					jsonTexts  = JsonTree(labelJSON.getChild("design"));
					for(auto it : jsonTexts)
					{
						string lang	= it.getChild("lang").getValue<string>();	
						data.configTexts.insert(lang, TextID::DESIGNMAIN,	jtools().parseTextItem(it.getChild("designMain")));
						data.configTexts.insert(lang, TextID::DESIGNSUB,	jtools().parseTextItem(it.getChild("designSub")));					
					}

					jsonTexts  = JsonTree(labelJSON.getChild("stat"));
					for(auto it : jsonTexts)
					{
						string lang	= it.getChild("lang").getValue<string>();	
						data.configTexts.insert(lang, TextID::FINANDSTART,	jtools().parseTextItem(it.getChild("finAndStart")));
						data.configTexts.insert(lang, TextID::PLAYED_COUNT,	jtools().parseTextItem(it.getChild("playedCount")));
						data.configTexts.insert(lang, TextID::PRINTED_COUNT,   jtools().parseTextItem(it.getChild("printedCount")));
						data.configTexts.insert(lang, TextID::SOCIAL_COUNT,	jtools().parseTextItem(it.getChild("socialCount")));						
					}

					jsonTexts  = JsonTree(labelJSON.getChild("screensaver"));
					for(auto it : jsonTexts)
					{
						string lang	= it.getChild("lang").getValue<string>();	
						data.configTexts.insert(lang, TextID::LOADMAIN,	 jtools().parseTextItem(it.getChild("loadMain")));
						data.configTexts.insert(lang, TextID::LOADSUB, jtools().parseTextItem(it.getChild("loadSub")));					
					}

					jsonTexts  = JsonTree(labelJSON.getChild("screensaver_back"));
					for(auto it : jsonTexts)
					{
						string lang	= it.getChild("lang").getValue<string>();	
						data.configTexts.insert(lang, TextID::BACK, jtools().parseTextItem(it.getChild("back")));
					}

					jsonTexts = JsonTree(labelJSON.getChild("load"));
					for(auto it : jsonTexts)
					{
						string lang	= it.getChild("lang").getValue<string>();	
						data.configTexts.insert(lang, TextID::LOAD, jtools().parseTextItem(it.getChild("load")));
					}

					jsonTexts = JsonTree(labelJSON.getChild("printer"));
					for(auto it : jsonTexts)
					{
						string lang	     = it.getChild("lang").getValue<string>();										
						data.configTexts.insert(lang, TextID::PHOTO_LEFT, jtools().parseTextItem(it.getChild("catridgeChange")));
						data.configTexts.insert(lang, TextID::CATRINGE_CHANGED, jtools().parseTextItem(it.getChild("changed")));
						data.configTexts.insert(lang, TextID::PRINTER_ASK_TITLE, jtools().parseTextItem(it.getChild("changeTitle")));
						data.configTexts.insert(lang, TextID::PRINTER_ASK_SUB_TITLE, jtools().parseTextItem(it.getChild("changeSubTitle")));
						data.configTexts.insert(lang, TextID::PRINTER_CLOSE_TITLE, jtools().parseTextItem(it.getChild("closeTitle")));
						data.configTexts.insert(lang, TextID::PRINTER_OK_TITLE, jtools().parseTextItem(it.getChild("okTitle")));
					}

					jsonTexts = JsonTree(labelJSON.getChild("newParty"));
					for(auto it : jsonTexts)
					{
						string lang	     = it.getChild("lang").getValue<string>();										
						data.configTexts.insert(lang, TextID::PARTY_ASK_TITLE, jtools().parseTextItem(it.getChild("askTitle")));
						data.configTexts.insert(lang, TextID::PARTY_DESQR, jtools().parseTextItem(it.getChild("desqr")));
						data.configTexts.insert(lang, TextID::PARTY_TITLE, jtools().parseTextItem(it.getChild("title")));
						data.configTexts.insert(lang, TextID::PARTY_BEGIN, jtools().parseTextItem(it.getChild("begin")));
						data.configTexts.insert(lang, TextID::PARTY_CLOSE, jtools().parseTextItem(it.getChild("close")));		
					}					
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
				addToDictionary("introBold90",		 createFontResource(getFontsPath("introb.ttf"), 90));
				addToDictionary("introBold80",		 createFontResource(getFontsPath("introb.ttf"), 80));
				addToDictionary("introBold70",		 createFontResource(getFontsPath("introb.ttf"), 70));
				addToDictionary("introBold60",		 createFontResource(getFontsPath("introb.ttf"), 60));


				addToDictionary("introBold72",		 createFontResource(getFontsPath("introb.ttf"), 72));
				addToDictionary("introLight44",		 createFontResource(getFontsPath("IntroLight.ttf"), 44));
				addToDictionary("introLight36",		 createFontResource(getFontsPath("IntroLight.ttf"), 36));
				addToDictionary("introLight72",		 createFontResource(getFontsPath("IntroLight.ttf"), 72));
				addToDictionary("introLight30",		 createFontResource(getFontsPath("IntroLight.ttf"), 30));	

				addToDictionary("iconClose",		 createImageResource(getDesignPath() + "closeConfig.png"));
				addToDictionary("iconBack",			 createImageResource(getDesignPath() + "backIcon.png"));
				addToDictionary("iconStartNew",		 createImageResource(getDesignPath() + "iconStartNew.png"));
				addToDictionary("designIcon",		 createImageResource(getDesignPath() + "ssIcon.png"));			
				addToDictionary("catridgeIcon",		 createImageResource(getDesignPath() + "catridgeIcon.png"));
				addToDictionary("catridgeIcon",		 createImageResource(getDesignPath() + "catridgeIcon.png"));
				addToDictionary("logoIcon",			 createImageResource(getDesignPath() + "logo.png"));
				addToDictionary("circles",			 createImageResource(getDesignPath() + "circles.png"));
				addToDictionary("loadIcon",			 createImageResource(getDesignPath() + "loadIcon.png"));
				addToDictionary("closePrinterIcon",	 createImageResource(getDesignPath() + "main/closePrinterIcon.png"));
				addToDictionary("inputField",		 createImageResource(getDesignPath() + "main/inputField.png"));
				addToDictionary("ssCheckerOn",		 createImageResource(getDesignPath() + "main/ssCheckerOn.png"));
				addToDictionary("ssCheckerOff", createImageResource(getDesignPath() + "main/ssCheckerOff.png"));
				addToDictionary("redFocus", createImageResource(getDesignPath() + "main/redFocus.png"));


				addToDictionary("gameInWebIcon",	 createImageResource(getDesignPath() + "gameInWebIcon.png"));

				for (auto item : data.designData)			
					addToDictionary(item.getIconTexName(),	 createImageResource(getDesignPath() + item.getIconPath()));
			}

			void buildData()
			{
				logger().log("main config build");
				auto texts = data.getTexts();
				auto dic = texts.getDic();

				logger().log(to_string(dic.size()));

				for (ConfigTexts<TextID>::DicType::iterator it = dic.begin(); it != dic.end(); ++it)	
				{
					it->second.setFont(fonts);					
				}

				for (auto it = data.designData.begin(); it != data.designData.end(); ++it)
				{
					it->setIcon(getTexture(it->getIconTexName()));
					it->setFont(fonts);
				}

				texts.setDic(dic);		
				data.setTexts(texts);
			}			

			void writeConfig()
			{
				if (memento)
				{					
					fs::path basePath(mainConfigObj.getParamsConfigPath());
					JsonTree doc;

					doc.addChild(JsonTree("designPath", designPath));		
					doc.addChild(JsonTree("actionName", data.actionName));		
					doc.addChild(JsonTree("playedCount", data.playedCount));		
					doc.addChild(JsonTree("printedCount", data.printedCount));		
					doc.addChild(JsonTree("publishedCount", data.publishedCount));		
					doc.addChild(JsonTree("currentPhotosPrinted", data.currentPhotosPrinted));		
					doc.addChild(JsonTree("maxPhotosToPrint", data.maxPhotosToPrint));		
					doc.addChild(JsonTree("activeDesign", data.activeDesign));
					doc.write( writeFile(basePath), JsonTree::WriteOptions());

					memento = false;
					logger().log("MAIN CONFIG CHANGED!!!!");					
				}				
			}

			bool settingsChanged()
			{
				return (data.actionName			  != mementoData.actionName ||
						data.playedCount		  != mementoData.playedCount ||
						data.printedCount		  != mementoData.printedCount ||
						data.publishedCount		  != mementoData.publishedCount ||
						data.currentPhotosPrinted != mementoData.currentPhotosPrinted ||
						data.activeDesign		  != mementoData.activeDesign);					
			}

			bool designChanged()
			{
				return data.activeDesign != mementoData.activeDesign;
			}

			changeSetting::id getChangeID() const
			{
				return changeSetting::id::CONFIG;
			}
		};

		typedef ConfigSettings::TextID ConfigTextID;
		typedef std::shared_ptr<ConfigSettings> ConfigSettingsRef;	
	}
}