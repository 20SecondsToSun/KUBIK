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
	namespace twParams
	{
		const std::string STATUS_DEFAULT = "#KUBIK";
	}

	typedef std::shared_ptr<class Twitter> TwitterRef;

	class Twitter : public SocShare
	{
	public:
		Twitter(){ isAuthFlowComplete = false;  twitterAuthURL = "https://twitter.com"; };

		bool							authorize(const std::string& login, const std::string& password);
		bool							postTextTweet(const std::string& status);
		bool							postPhotoTweet(const std::string& status, const std::vector<std::string>& filesPath);
		bool							postPhotoTweetBase64(const std::string& status, const std::string& filesPath);

		const char *					getAuthUrl();
		void							post();
		void							logOut();

	private:

		bool							isAuthFlowComplete;
		twitCurl						twitterObj;
		std::shared_ptr<std::thread>	serverThread;
		void							twitterPostThread();
		std::string						twitterAuthURL;
		std::string						getDefaultStatus();		
	};
}