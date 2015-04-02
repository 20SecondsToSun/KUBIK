#pragma once

#include "ISettings.h"
#include "ConfigSettings.h"

using namespace kubik::config;

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class FuncesSettings> FuncesSettingsRef;

		class FuncesSettings : public ISettings
		{
			ConfigSettingsRef configSettings;

		public:
			FuncesSettings(ApplicationModelRef model, ConfigSettingsRef configSettings)
				:ISettings(model), configSettings(configSettings)
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
			bool settingsChanged(){ return false; };
			changeSetting::id getChangeID() const { return changeSetting::id::FUNCES; };
		};
	}
}