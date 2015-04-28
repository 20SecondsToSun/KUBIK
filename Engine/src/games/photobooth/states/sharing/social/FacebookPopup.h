#pragma once
#include "states/sharing/social/SocialPopup.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class FacebookPopup> FacebookPopupRef;

			class FacebookPopup : public SocialPopup
			{
				void createSocialContext() override;

			public:
				FacebookPopup(kubik::config::PhotoboothSettingsRef settings);
			};
		}
	}
}