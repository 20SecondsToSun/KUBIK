#pragma once

#include "services/social/Vkontakte.h"
#include "services/social/Facebook.h"
#include "services/social/Twitter.h"
#include "services/social/SocShare.h"

namespace kubik
{
	class Social
	{
		static VkontakteRef vkontakte;
		static FacebookRef facebook;
		static TwitterRef twitter;

	public:
		static SocShareRef createVkontakte()
		{
			if (!vkontakte)
				vkontakte = VkontakteRef(new Vkontakte());
			return vkontakte;
		}

		static SocShareRef createFacebook()
		{
			if (!facebook)
				facebook = FacebookRef(new Facebook());
			return facebook;
		}

		static SocShareRef createTwitter()
		{
			if (!twitter)
				twitter = TwitterRef(new Twitter());
			return twitter;
		}		
	};
}