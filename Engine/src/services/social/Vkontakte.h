#pragma once
#include "services/social/SocShare.h"
#include "SocialSettings.h"

namespace kubik
{
	typedef std::shared_ptr<class Vkontakte> VkontakteRef;

	class Vkontakte : public SocShare
	{
	public:
		Vkontakte();
		
		std::string getAuthUrl();
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
