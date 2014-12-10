#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "ISettings.h"
#include "ApplicationModel.h"
#include "KubikException.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class TuneUpSettings:public ISettings
	{
	public:
		TuneUpSettings(shared_ptr<ApplicationModel> model)
		{
			this->model = model;
			configPath = model->getTuneUpConfigPath();

			load();
			setTextures();
		}

		void load() override
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

		void setTextures()
		{
			addToDictionary("helvetica90",  getFontsPath()  + "Helvetica Neue.ttf", resourceType::FONT, loadingType::FULL_PATH, 90);
			addToDictionary("helvetica20",  getFontsPath()  + "Helvetica Neue.ttf", resourceType::FONT, loadingType::FULL_PATH, 20);
			addToDictionary("closeImg",		getDesignPath() + "close.png");
		}
		
	};
}