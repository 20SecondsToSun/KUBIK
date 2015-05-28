#include "states/sharing/social/VkontaktePopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

VkontaktePopup::VkontaktePopup(PhotoboothSettingsRef settings) : SocialPopup(settings)
{
	createSocialContext();
}

void VkontaktePopup::createSocialContext()
{
	social = Social::createVkontakte();	
}