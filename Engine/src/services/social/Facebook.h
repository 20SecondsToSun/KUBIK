#pragma once
#include "services/social/SocShare.h"
#include "Socialsettings.h"

namespace kubik
{
	typedef std::shared_ptr<class Facebook> FacebookRef;

	class Facebook : public SocShare
	{
	public:
		Facebook();

		std::string getAuthUrl();
		void logOut();

		void updatePopupPosition() override;
		void update() override;
		std::string	getDefaultStatus() override;
		int getBrowserWidth() override;
		int getBrowserHeight() override;

	private:
		void postTextFB();
		void facebookPostThread();
		void waitLoadingComplete();
		void posting();
	};
}