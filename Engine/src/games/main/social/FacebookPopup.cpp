#include "main/social/FacebookPopup.h"

using namespace kubik::games::photobooth;
using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;

FacebookPopup::FacebookPopup(ISettingsRef settings, StatCollectorRef statSettings)
	:SocialPopup(settings),
	set(settings),
	statSettings(statSettings)
{
	sharingType = SharingType::FB;
	createSocialContext();
}

void FacebookPopup::createSocialContext()
{
	social = Social::createFacebook();
}

void FacebookPopup::postingCompleteHandler()
{
	logger().log("fb shared");
	//social->getLinkToPost();
	statSettings->addFBShare();	
	
	SocialPopup::postingCompleteHandler();	
}
