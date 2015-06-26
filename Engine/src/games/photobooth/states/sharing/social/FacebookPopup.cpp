#include "states/sharing/social/FacebookPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

FacebookPopup::FacebookPopup(PhotoboothSettingsRef settings):SocialPopup(settings)
{
	createSocialContext();
}

void FacebookPopup::createSocialContext()
{
	social = Social::createFacebook();
}