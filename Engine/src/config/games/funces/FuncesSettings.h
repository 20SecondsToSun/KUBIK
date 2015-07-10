#pragma once

#include "ISettings.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class FuncesSettings> FuncesSettingsRef;

		class FuncesSettings : public ISettings
		{
			ConfigSettingsRef configSettings;
			ConfigObject mainConfigObj;

			void setDesignPath();
			void loadPaths();

			std::string finalPath;

		public:
			FuncesSettings(ApplicationModelRef model, ConfigSettingsRef configSettings);

			virtual void createMemento();
			virtual void writeConfig();

			void load() override;
			void setTextures() override;
			bool settingsChanged();

			changeSetting::id getChangeID() const;
		};
	}
}