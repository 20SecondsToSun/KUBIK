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
		void				logOut() override;
		void				update() override;

	private:
		std::string			vkontakteAuthURL;

		void				vkontaktePostThread();
		void				postText(const std::string& textStatus) override;
		void				postPhoto(const std::string& textStatus, const std::vector<std::string>& filesPath) override;		
		void				signInUpdate();	
		std::string			vkontaktePostLoadPhotoPath(const std::string& upload_url, const std::string& path);
		std::string			getPostingStatus() const override;
	};
}