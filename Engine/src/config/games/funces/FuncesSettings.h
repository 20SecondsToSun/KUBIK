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

		FuncesSettings(ApplicationModelRef model):ISettings(model)
		{
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

		virtual void createMemento(){};
		virtual void writeConfig(){};
		bool settingsChanged(){return false;};	
		changeSetting::id getChangeID(){ return changeSetting::id::FUNCES;};	
	};

	typedef shared_ptr<FuncesSettings> FuncesSettingsRef;
}