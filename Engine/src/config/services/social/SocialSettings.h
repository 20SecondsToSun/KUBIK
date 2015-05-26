#pragma once

#include "ISettings.h"
#include "ApplicationModel.h"
#include "KubikException.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class SocialSettings> SocialSettingsRef;

		class SocialSettings :public ISettings
		{
		public:
			SocialSettings(ApplicationModelRef model);

			virtual void load();
			virtual void setTextures(){};
			virtual void createMemento(){};
			virtual void writeConfig(){};
			virtual bool settingsChanged(){ return false; };
			virtual changeSetting::id getChangeID() const ;				

			static const std::string FACEBOOK_BASE_URL;
			static const std::string FACEBOOK_FEED_URL;
			static const std::string FACEBOOK_APP_LOGOUT_URL;
			static const std::string FACEBOOK_ALBUMS_URL;
			static const std::string FACEBOOK_ACCESS_TOKEN;
			static const std::string FACEBOOK_NULL_ALBUM_ID;
			static const std::string FACEBOOK_ALBUM_NAME_DEFAULT;
			static const std::string FACEBOOK_STATUS_DEFAULT;
			static const std::string FACEBOOK_POSTING_WAITING_TEXT;
			static std::string FACEBOOK_AUTH_URL; 

			static std::string TWITTER_TOKEN_KEY;
			static std::string TWITTER_TOKEN_SECRET;
			static std::string TWITTER_API_KEY;
			static std::string TWITTER_API_SECRET;
			static const std::string TWITTER_AUTH_URL;	
			static const std::string TWITTER_STATUS_DEFAULT;
			static const std::string TWITTER_OAUTH_CALLBACK;

			static std::string VK_APP_ID;
			static std::string VK_AUTH_URL;
			static std::string VK_LOGOUT_URL;

			static const std::string VK_BASE_URL;
			static const std::string VK_WALL_POST_URL;
			static const std::string VK_WALL_UPLOAD_SERVER_URL;
			static const std::string VK_SAVE_WALL_PHOTO_URL;

			static const std::string VK_ACCESS_TOKEN;
			static const std::string VK_NULL_ALBUM_ID;
			static const std::string VK_STATUS_DEFAULT;
			static const std::string VK_POSTING_WAITING_TEXT;

		private:
			static std::string FACEBOOK_APP_ID;			
		};
	}
}