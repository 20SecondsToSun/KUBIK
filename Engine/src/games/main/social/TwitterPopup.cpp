#include "main/social/TwitterPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

TwitterPopup::TwitterPopup(ISettingsRef settings, kubik::config::StatCollectorRef statSettings)
	:SocialPopup(settings),
	statSettings(statSettings),
	set(settings)
{
	sharingType = SharingType::TW;
	createSocialContext();
}

void TwitterPopup::createSocialContext()
{
	social = Social::createTwitter();
}

void TwitterPopup::postingCompleteHandler()
{
	logger().log("tw shared");
	statSettings->addTWShare();	
	SocialPopup::postingCompleteHandler();
}
