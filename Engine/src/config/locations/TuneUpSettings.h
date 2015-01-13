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
				setTextures();
			}
			catch(...)
			{
				throw ExcConfigFileParsing();
			}
		}

		void setTextures()
		{
			addToDictionary("helvetica90",  createFontResource(getFontsPath("Helvetica Neue.ttf"), 90));
			addToDictionary("helvetica20",  createFontResource(getFontsPath("Helvetica Neue.ttf"), 20));
			addToDictionary("closeImg",		createImageResource(getDesignPath() +"close.png"));
		}		
	};

	typedef shared_ptr<ConfigSettings> ConfigSettingsRef;	
}