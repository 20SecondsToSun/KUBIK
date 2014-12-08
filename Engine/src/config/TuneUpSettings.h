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
		TuneUpSettings(ApplicationModel *model)
		{
			this->model = model;
			load(model);
		}

		void load(ApplicationModel *model)
		{
			try	
			{
				JsonTree configJSON = JsonTree(loadFile(model->getTuneUpConfigPath()));
				designPath = configJSON.getChild("designPath").getValue<string>();
			}
			catch(...)
			{
				throw ExcConfigFileParsing();
			}
		}

	private:	
		ApplicationModel *model;

	};
}