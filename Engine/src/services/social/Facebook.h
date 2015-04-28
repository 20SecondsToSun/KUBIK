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

		const char *getAuthUrl();
		void logOut();

		virtual void updatePopupPosition() override;
		virtual void update() override;
		virtual std::string	getDefaultStatus() override;

	private:
		void postTextFB();
		void facebookPostThread();
		void waitLoadingComplete();
		void posting();
	};
}