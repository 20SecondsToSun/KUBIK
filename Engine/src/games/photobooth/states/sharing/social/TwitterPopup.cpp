#include "states/sharing/social/TwitterPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

TwitterPopup::TwitterPopup(PhotoboothSettingsRef settings)
	:SocialPopup(settings),
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
	set->addTWShare();
	SocialPopup::postingCompleteHandler();
}

