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
			mainConfigPath = model->getTuneUpConfigPath();

			load();
			setTextures();
		}

		void load() override
		{
			try	
			{
				JsonTree configJSON = JsonTree(loadFile(mainConfigPath));
				designPath = configJSON.getChild("designPath").getValue<string>();
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
}