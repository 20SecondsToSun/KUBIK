#pragma once

#include "Checker.h"
#include "CheckerSocialEvent.h"
#include "PhotoboothSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class CheckerSocial> CheckerSocialRef;

		class CheckerSocial: public Checker
		{
		public:
			CheckerSocial(const Rectf& rect, const IconPair& icons, const PhotoboothSettings::PhtTextID& id);
			virtual void mouseUp(ci::app::MouseEvent &event);
		};
	}
}