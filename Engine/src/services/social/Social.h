#pragma once

#include "services/social/Vkontakte.h"
#include "services/social/Facebook.h"
#include "services/social/SocShare.h"

namespace kubik
{
	class Social
	{
		static VkontakteRef vkontakte;
		static FacebookRef facebook;

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
	};
}