#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "KubikException.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class MenuSettings:public ISettings
	{

	public:
		struct MenuDataStruct
		{
			bool isCustomDesign;
			int  templateId;

			string staticPartDesignPath;
			string kubikTemplatePartDesignPath;
			string userTemplatePartDesignPath;
			string finalPath;

			bool hasDesignChanges(MenuDataStruct menu)
			{
				return (isCustomDesign != menu.isCustomDesign ||
						    templateId != menu.templateId);
			}		
		};

		MenuSettings(ApplicationModelRef model):ISettings(model)
		{
			mainConfigPath  = model->getMenuConfigPath();
			//load();		
		}

		void load()
		{
			logger().log("menu settings load");

			JsonTree configJSON					= JsonTree(loadFile(mainConfigPath));
			data.staticPartDesignPath			= configJSON.getChild("staticPartDesignPath").getValue<string>();
			data.kubikTemplatePartDesignPath	= configJSON.getChild("kubikTemplatePartDesignPath").getValue<string>();
			data.userTemplatePartDesignPath		= configJSON.getChild("userTemplatePartDesignPath").getValue<string>();
			data.finalPath						= configJSON.getChild("finalPath").getValue<string>();
			data.templateId						= configJSON.getChild("templateId").getValue<int>();
			data.isCustomDesign					= configJSON.getChild("isCustomDesign").getValue<bool>();
			
			setDesignPath();
			setTextures();
		}

		void saveConfig()
		{
			console()<<"SAVE MENU CONFIG"<<endl;

			fs::path basePath(mainConfigPath);

			JsonTree doc;		
			doc.addChild( JsonTree("staticPartDesignPath", data.staticPartDesignPath));
			doc.addChild( JsonTree("kubikTemplatePartDesignPath", data.kubikTemplatePartDesignPath));
			doc.addChild( JsonTree("userTemplatePartDesignPath", data.userTemplatePartDesignPath));
			doc.addChild( JsonTree("finalPath", data.finalPath));
			doc.addChild( JsonTree("templateId", data.templateId));
			doc.addChild( JsonTree("isCustomDesign", data.isCustomDesign));	
			doc.write( writeFile(basePath), JsonTree::WriteOptions() );
		}

		virtual void createMemento(){};
		virtual void writeConfig(){};
		bool settingsChanged(){return false;};	
		changeSetting::id getChangeID(){ return changeSetting::id::MENU;};	

		void setDesignPath()
		{
			if(data.isCustomDesign)
				templateDesignPath = data.userTemplatePartDesignPath + to_string(data.templateId)+"\\" + data.finalPath;
			else
				templateDesignPath = data.kubikTemplatePartDesignPath + to_string(data.templateId)+"\\" + data.finalPath;

			staticDesignPath = data.staticPartDesignPath + data.finalPath;
		}			
		
		void setTextures()
		{		
			clearResources();
			addToDictionary("background",	createImageResource(getTemplateDesignPath("bg.jpg")));
			addToDictionary("menuButton",	createImageResource(getTemplateDesignPath("menulayer.png")));
			addToDictionary("helvetica30",  createFontResource(getFontsPath("Helvetica Neue.ttf"), 30));
		}

		void setData(MenuDataStruct value)
		{
			data = value;
			saveConfig();
		}

		MenuDataStruct getData()
		{
			return data;
		}

		vector<GamesInfo> getGames()
		{
			return model->getGames();
		}

		string getUserDesighFullPath()
		{
			return getBasePath().string() + data.userTemplatePartDesignPath + to_string(data.templateId)+ "\\" + data.finalPath;			
		}

		string getKubikDesighFullPath()
		{
			return getBasePath().string() + data.kubikTemplatePartDesignPath + to_string(data.templateId)+ "\\" + data.finalPath;			
		}		

		private:
			MenuDataStruct data;	
	};

	typedef shared_ptr<MenuSettings> MenuSettingsRef;
}