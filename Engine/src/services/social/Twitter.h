#pragma once

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

		void		postTextTweet(const std::string& status);
		void		postPhotoTweet(const std::string& status, const std::vector<std::string>& filesPath);
		void		postPhotoTweetBase64(const std::string& status, const std::string& filesPath);
	
		std::string	getAuthUrl() override;
		void		post();
		void		logOut() override;
		void		update() override;
		
		int			getBrowserWidth() const override;
		int			getBrowserHeight() const override;
		
	private:
		bool		isAuthFlowComplete;
		twitCurl	twitterObj;
		std::string	authURL;

		std::string getDefaultStatus() const override;	
		void		initChromium() override;
		void		initChromiumThread();

		void		getAuthUpdate();
		void		signInUpdate();
		bool		userRejectedAppOauth();

		std::string getPostingStatus() const;
		std::vector<std::string> getUploadPhotoPathVec() const;
	};
}