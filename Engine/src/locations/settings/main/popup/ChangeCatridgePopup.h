#pragma once

#include "main/popup/MainPopup.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ChangeCatridgePopup> ChangeCatridgePopupRef;

		class NewActivityPopup: public MainPopup
		{
		public:	
			ChangeCatridgePopup(ConfigSettingsRef configSett):MainPopup(configSett)
			{

			}
		};
	}
}