#pragma once

#include "ISettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class FuncesSettings: public ISettings
	{
	public:

		FuncesSettings(shared_ptr<ApplicationModel> model) 
		{
			this->model = model;
			mainConfigPath = model->getFuncesConfigPath();
			
		}

		void load() override
		{
			JsonTree configJSON = JsonTree(loadFile(mainConfigPath));
			designPath = configJSON.getChild("designPath").getValue<string>();
			setTextures();
		}

		void setTextures() override
		{		
			clearResources();
			addToDictionary("closeImg", createImageResource(getDesignPath() + "close.png"));
		}	

	};
}