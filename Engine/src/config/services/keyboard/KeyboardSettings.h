#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "KubikException.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class KeyboardSettings> KeyboardSettingsRef;

		class KeyboardSettings:public ISettings
		{
		public:
			KeyboardSettings(ApplicationModelRef model);

			virtual void load();
			virtual void setTextures();
			virtual void createMemento(){};
			virtual void writeConfig(){};
			virtual bool settingsChanged(){ return false; };
			virtual changeSetting::id getChangeID() const;

		private:
			std::string getKeyboardPath(const string& value);
		};
	}
}