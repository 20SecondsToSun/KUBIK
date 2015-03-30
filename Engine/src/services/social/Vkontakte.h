#pragma once
#include "services/social/SocShare.h"

namespace kubik
{
	typedef std::shared_ptr<class Vkontakte> VkontakteRef;

	class Vkontakte : public SocShare
	{
	public:
		static const std::string VKONTAKTE_APP_ID;
		static const std::string VKONTAKTE_AUTH_URL;
		static const std::string VKONTAKTE_LOGOUT_URL;

		static const std::string VKONTAKTE_BASE_URL;
		static const std::string WALL_POST_URL;
		static const std::string WALL_UPLOAD_SERVER_URL;
		static const std::string SAVE_WALL_PHOTO_URL;

		static const std::string ACCESS_TOKEN;
		static const std::string NULL_ALBUM_ID;
		static const std::string STATUS_DEFAULT;
		static const std::string POSTING_WAITING_TEXT;

		Vkontakte();
		
		const char *getAuthUrl();
		void logOut();
		virtual void updatePopupPosition() override;
		virtual void update() override;

	private:
		std::string					 vkontakteAuthURL;
		std::shared_ptr<std::thread> serverThread;

		void						 vkontaktePostThread();
		void						 postTextVK();
		void						 postPhotoVK();		
		std::string					 vkontaktePostLoadPhotoPath(const std::string& upload_url, const std::string& path);
		virtual std::string	 getDefaultStatus() override;

		void  waitLoadingComplete();
		void  posting();
	};
}
