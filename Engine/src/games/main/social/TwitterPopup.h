#pragma once

#include "main/social/SocialPopup.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class TwitterPopup> TwitterPopupRef;

			class TwitterPopup : public SocialPopup
			{
				kubik::config::ISettingsRef set;
				kubik::config::StatCollectorRef statSettings;

				void createSocialContext() override;
				virtual void postingCompleteHandler() override;

			public:
				TwitterPopup(kubik::config::ISettingsRef settings, kubik::config::StatCollectorRef statSettings);
			};
		}
	}
}
