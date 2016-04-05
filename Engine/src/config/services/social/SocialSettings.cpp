#include "SocialSettings.h"

using namespace kubik;
using namespace kubik::config;

////////////////////////////////////////////////////////////////////////////
//
//				FACEBOOK
//
////////////////////////////////////////////////////////////////////////////

const std::string SocialSettings::FACEBOOK_BASE_URL = "https://graph.facebook.com/";

const std::string SocialSettings::FACEBOOK_FEED_URL = FACEBOOK_BASE_URL + "me/feed";
const std::string SocialSettings::FACEBOOK_APP_LOGOUT_URL = FACEBOOK_BASE_URL + "me/permissions";
const std::string SocialSettings::FACEBOOK_ALBUMS_URL = FACEBOOK_BASE_URL + "me/albums";

const std::string SocialSettings::FACEBOOK_ACCESS_TOKEN = "access_token";
const std::string SocialSettings::FACEBOOK_NULL_ALBUM_ID = "-1";
const std::string SocialSettings::FACEBOOK_ALBUM_NAME_DEFAULT = "Dreamies";
const std::string SocialSettings::FACEBOOK_STATUS_DEFAULT = "#тест рич бич";
const std::string SocialSettings::FACEBOOK_POSTING_WAITING_TEXT = "Facebook..";

std::string SocialSettings::FACEBOOK_APP_ID;
std::string SocialSettings::FACEBOOK_AUTH_URL;

////////////////////////////////////////////////////////////////////////////
//
//				TWITTER
//
////////////////////////////////////////////////////////////////////////////

std::string SocialSettings::TWITTER_TOKEN_KEY;
std::string SocialSettings::TWITTER_TOKEN_SECRET;
std::string SocialSettings::TWITTER_API_KEY;
std::string SocialSettings::TWITTER_API_SECRET;
const std::string SocialSettings::TWITTER_AUTH_URL = "https://twitter.com";
const std::string SocialSettings::TWITTER_STATUS_DEFAULT = "#тест rich bich";
const std::string SocialSettings::TWITTER_OAUTH_CALLBACK = ",oauth_callback=http://familyagency.ru";

////////////////////////////////////////////////////////////////////////////
//
//				VKONTAKTE
//
////////////////////////////////////////////////////////////////////////////

std::string SocialSettings::VK_APP_ID;
std::string SocialSettings::VK_AUTH_URL;
std::string SocialSettings::VK_LOGOUT_URL;

const std::string SocialSettings::VK_BASE_URL = "https://api.vk.com/";
const std::string SocialSettings::VK_WALL_POST_URL = VK_BASE_URL + "method/wall.post";
const std::string SocialSettings::VK_WALL_UPLOAD_SERVER_URL = VK_BASE_URL + "method/photos.getWallUploadServer";
const std::string SocialSettings::VK_SAVE_WALL_PHOTO_URL = VK_BASE_URL + "method/photos.saveWallPhoto";

const std::string SocialSettings::VK_ACCESS_TOKEN = "access_token";
const std::string SocialSettings::VK_NULL_ALBUM_ID = "-1";
const std::string SocialSettings::VK_STATUS_DEFAULT = "#тест rich bich";
const std::string SocialSettings::VK_POSTING_WAITING_TEXT = "...";

SocialSettings::SocialSettings(ApplicationModelRef model) :ISettings(model)
{

}

void SocialSettings::load()
{
	auto basePath		 = getBasePath() / model->getSocialSettingsFilePath();
	JsonTree socialJSON  = JsonTree(loadFile(basePath));
	FACEBOOK_APP_ID		 = socialJSON.getChild("facebookAppId").getValue<string>();
	FACEBOOK_AUTH_URL    = "https://www.facebook.com/dialog/oauth?client_id=" + FACEBOOK_APP_ID + "&redirect_uri=http://familyagency.ru/&response_type=token&display=popup&scope=publish_actions,email,public_profile,user_friends,user_photos";

	TWITTER_TOKEN_KEY    = socialJSON.getChild("twitterTokenKey").getValue<string>();
	TWITTER_TOKEN_SECRET = socialJSON.getChild("twitterTokenSecret").getValue<string>();
	TWITTER_API_KEY		 = socialJSON.getChild("twitterApiKey").getValue<string>();
	TWITTER_API_SECRET   = socialJSON.getChild("twitterApiSecret").getValue<string>();

	VK_APP_ID			 = socialJSON.getChild("vkontakteAppId").getValue<string>();
	VK_AUTH_URL			 = "https://oauth.vk.com/authorize?client_id=" + VK_APP_ID + "&redirect_uri=https://oauth.vk.com/blank.html&scope=photos,wall&display=page&response_type=token";
	VK_LOGOUT_URL		 = "https://login.vk.com/?act=openapi&oauth=1&aid=" + VK_APP_ID + "&location=familyagency.ru&do_logout=1&token=";
}

changeSetting::id SocialSettings::getChangeID() const
{
	return changeSetting::id::SOCIAL;
}

void SocialSettings::setTextures()
{

}

void SocialSettings::createMemento()
{

}

void SocialSettings::writeConfig()
{

}

bool SocialSettings::settingsChanged()
{
	return false;
}
