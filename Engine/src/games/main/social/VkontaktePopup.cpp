#include "main/social/VkontaktePopup.h"

using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

VkontaktePopup::VkontaktePopup(ISettingsRef settings, kubik::config::StatCollectorRef statSettings)
	:SocialPopup(settings),
	statSettings(statSettings),
	set(settings)
{
	sharingType = SharingType::VK;
	createSocialContext();
}

void VkontaktePopup::createSocialContext()
{
	social = Social::createVkontakte();	
}

void VkontaktePopup::postingCompleteHandler()
{
	logger().log("vk shared");
	
	statSettings->addVKShare();	
	SocialPopup::postingCompleteHandler();
}

