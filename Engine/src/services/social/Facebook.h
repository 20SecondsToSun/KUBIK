#pragma once
#include "services/social/SocShare.h"

namespace kubik
{
	typedef std::shared_ptr<class Facebook> FacebookRef;

	class Facebook : public SocShare
	{
	public:
		static const std::string FACEBOOK_BASE_URL;

		static const std::string FACEBOOK_APP_ID;
		static const std::string FACEBOOK_AUTH_URL;

		static const std::string FACEBOOK_FEED_URL;
		static const std::string FACEBOOK_APP_LOGOUT_URL;
		static const std::string FACEBOOK_ALBUMS_URL;
	
		static const std::string ACCESS_TOKEN;
		static const std::string NULL_ALBUM_ID;
		static const std::string ALBUM_NAME_DEFAULT;
		static const std::string STATUS_DEFAULT;
		static const std::string POSTING_WAITING_TEXT;

		Facebook();

		const char *getAuthUrl();
		void logOut();
		virtual void updatePopupPosition() override;
		virtual void update() override;
		virtual std::string	 getDefaultStatus() override;

	private:
		void postTextFB();
		void facebookPostThread();
		void waitLoadingComplete();
		void posting();
	};
}