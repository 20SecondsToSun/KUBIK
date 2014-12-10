#pragma once
#include "cinder/app/AppNative.h"
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
		MenuSettings(shared_ptr<ApplicationModel> model)
		{
			this->model = model;
			configPath = model->getMenuConfigPath();

			load();
			setTextures();
		}

		void load()
		{
			try	
			{
				JsonTree configJSON = JsonTree(loadFile(configPath));
				designPath = configJSON.getChild("designPath").getValue<string>();
			}
			catch(...)
			{
				throw ExcConfigFileParsing();
			}
		}

		void setDesignPath(string path)
		{
			designPath = path;
		}

		vector<int> getGameIDs()
		{
			return model->getGameIDsTurnOn();
		}		

		void setTextures()
		{		
			designTexures.clear();
			addToDictionary("background",	getDesignPath() + "bg.jpg");
			addToDictionary("background1",  getDesignPath() + "title.jpg");
			addToDictionary("helvetica30",  getFontsPath()  + "Helvetica Neue.ttf", resourceType::FONT, loadingType::FULL_PATH, 30);
		}

	private:
		
		
	};
}