#pragma once
#include "states/sharing/social/SocialPopup.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class TwitterPopup> TwitterPopupRef;

			class TwitterPopup : public SocialPopup
			{
				void createSocialContext() override;

			public:
				TwitterPopup(kubik::config::PhotoboothSettingsRef settings);
			};
		}
	}
}