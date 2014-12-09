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
		MenuSettings(ApplicationModel *model)
		{
			console()<<"menu settings create"<<endl;
			this->model = model;

			load();
			setTextures();
		}

		void load()
		{
			try	
			{
				JsonTree configJSON = JsonTree(loadFile(model->getMenuConfigPath()));
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

		void reload()
		{
			setTextures();
		}

	private:

		ApplicationModel *model;

		void setTextures()
		{		
			designTexures.clear();
			addToDictionary("background",	getDesignPath() + "bg.jpg");
			addToDictionary("background1",  getDesignPath() + "title.jpg");
			addToDictionary("helvetica30",  getFontsPath()  + "Helvetica Neue.ttf", resourceType::FONT, loadingType::FULL_PATH, 30);
		}
	};
}