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
				void createSocialContext() override;

			public:
				VkontaktePopup(kubik::config::PhotoboothSettingsRef settings);				
			};
		}
	}
}