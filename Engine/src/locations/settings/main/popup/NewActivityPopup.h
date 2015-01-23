#pragma once

#include "main/popup/MainPopup.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class NewActivityPopup> NewActivityPopupRef;

		class NewActivityPopup: public MainPopup
		{
		public:	
			NewActivityPopup(ConfigSettingsRef configSett):MainPopup(configSett)
			{

			}
		};
	}
}