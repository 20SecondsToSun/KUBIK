#pragma once
#include "Checker.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ScreenSaverChecker> ScreenSaverCheckerRef;

		class ScreenSaverChecker: public Checker
		{
		public:
			ScreenSaverChecker(const ci::Rectf& rect, const IconPair& icons);
			virtual void drawLayout() override;
		};
	}
}