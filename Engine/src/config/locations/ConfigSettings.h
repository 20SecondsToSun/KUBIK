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

		class ConfigTexts
		{
		public:
			string getSwitchOffText()
			{
				return switchOffText;
			}
			
			string getConfigText()
			{
				return configText;
			}
		
			string getStatText()
			{
				return statText;
			}

			string getNotInstallGamesText()
			{
				return notInstallGamesText;
			}

			void  setSwitchOffText(string switchOffText)
			{
				this->switchOffText = switchOffText;
			}
			
			void setConfigText(string configText)
			{
				this->configText = configText;
			}
		
			void setStatText(string statText)
			{
				this->statText = statText;
			}

			void setNotInstallGamesText(string notInstallGamesText)
			{
				this->notInstallGamesText = notInstallGamesText;
			}

		private:
			string switchOffText, configText, statText, notInstallGamesText;
		};

		struct ConfighDataStruct
		{
			string actionName;
			int playedCount;
			int printedCount;
			int puplishedCount;
			int currentPhotosPrinted;
			int maxPhotosToPrint;
			std::map <string, ConfigTexts> btnTexts;
			string lang;

			void setLang(string lang)
			{
				this->lang = lang;
			}

			ConfigTexts getTexts()
			{
				return btnTexts[lang];
			}

			void setBtnTexts(string lang, ConfigTexts txts)
			{
				btnTexts[lang] = txts;
			}

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
				data.setLang(this->model->getLang());

				JsonTree texts = JsonTree(configJSON.getChild("texts"));
				for(auto it : texts)
				{
					string lang = it.getChild("lang").getValue<string>();
					ConfigTexts txts;
					txts.setSwitchOffText(it.getChild("text1").getValue<string>());
					txts.setConfigText(it.getChild("text2").getValue<string>());
					txts.setStatText(it.getChild("text3").getValue<string>());
					txts.setNotInstallGamesText(it.getChild("text4").getValue<string>());

					data.setBtnTexts(lang, txts);
				}

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
			addToDictionary("introLight36",  createFontResource(getFontsPath("IntroLight.ttf"), 36));

			addToDictionary("closeImg",		 createImageResource(getDesignPath() + "closeConfig.png"));
			addToDictionary("tempBg",		 createImageResource(getDesignPath() + "tempMain.jpg"));
			addToDictionary("iconStartNew",	 createImageResource(getDesignPath() + "iconStartNew.png"));
			addToDictionary("ssIcon",		 createImageResource(getDesignPath() + "ssIcon.png"));
			addToDictionary("menuIcon",		 createImageResource(getDesignPath() + "menuIcon.png"));

			addToDictionary("tempBottom",		 createImageResource(getDesignPath() + "tempBottom.png"));
			addToDictionary("catridgeIcon",		 createImageResource(getDesignPath() + "catridgeIcon.png"));
			addToDictionary("catridgeIcon",		 createImageResource(getDesignPath() + "catridgeIcon.png"));
			addToDictionary("logoIcon",			 createImageResource(getDesignPath() + "logo.png"));

			addToDictionary("appsTemp",		 createImageResource(getDesignPath() + "appsTemp.png"));
		}	

		string  getSwitchOffText()	
		{
			return data.getTexts().getSwitchOffText();
		}

		string  getConfigText()	
		{
			return data.getTexts().getConfigText();
		}

		string  getStatText()	
		{
			return data.getTexts().getStatText();
		}

		string  getNotInstallGamesText()	
		{
			return data.getTexts().getNotInstallGamesText();
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