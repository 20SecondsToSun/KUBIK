#pragma once

#include "main/social/SocialPopup.h"

namespace kubik
{
	namespace games
	{
		typedef	std::shared_ptr<class VkontaktePopup> VkontaktePopupRef;

		class VkontaktePopup : public SocialPopup
		{
			kubik::config::ISettingsRef set;
			kubik::config::StatCollectorRef statSettings;

			void createSocialContext() override;
			virtual void postingCompleteHandler() override;

		public:
			VkontaktePopup(kubik::config::ISettingsRef settings, kubik::config::StatCollectorRef statSettings);
		};		
	}
}