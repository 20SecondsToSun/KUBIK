#pragma once

#include "states/sharing/social/SocialPopup.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class VkontaktePopup> VkontaktePopupRef;

			class VkontaktePopup : public SocialPopup
			{
				kubik::config::PhotoboothSettingsRef set;

				void createSocialContext() override;
				virtual void postingCompleteHandler() override;

			public:
				VkontaktePopup(kubik::config::PhotoboothSettingsRef settings);				
			};
		}
	}
}