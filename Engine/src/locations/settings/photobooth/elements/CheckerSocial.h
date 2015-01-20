#pragma once
#include "Checker.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class CheckerSocial> CheckerSocialRef;

		class CheckerSocial: public Checker
		{
		public:
			CheckerSocial(IconPair icons):
				Checker(icons, Color::hex(0xeff0f3), Color::hex(0x773591))				
			{
				setRadius(33);
				setStartX(-6);
				setFinishX(48);
			}				
		};
	}
}