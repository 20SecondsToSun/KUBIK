#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "Utils.h"
#include "Curl.h"
#include "twitcurl.h"
#include "SocialSettings.h"
#include "services/social/SocShare.h"

namespace kubik
{
	typedef std::shared_ptr<class Twitter> TwitterRef;

	class Twitter : public SocShare
	{
	public:
		Twitter();

		bool							authorize(const std::string& login, const std::string& password);
		void							postTextTweet(const std::string& status);
		bool							postPhotoTweet(const std::string& status, const std::vector<std::string>& filesPath);
		bool							postPhotoTweetBase64(const std::string& status, const std::string& filesPath);
		void authorizePost(const std::string& login, const std::string& password, const std::string& status);

		std::string						getAuthUrl() override;
		void							post();
		void							logOut() override;
		void							update() override;
		void							updatePopupPosition() override;

		int getBrowserWidth() override;
		int getBrowserHeight() override;		

		boost::signals2::signal<void(void)> gettingAuthURLComplete;	

	private:

		bool							isAuthFlowComplete;
		twitCurl						twitterObj;
		std::shared_ptr<std::thread>	serverThread;
		void							twitterPostThread();
		std::string						getDefaultStatus() override;	
		void posting(const std::string& login, const std::string& password, const std::string& textstatus);
		void waitLoadingComplete();
	};
}