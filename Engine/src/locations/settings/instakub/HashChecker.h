#pragma once
#include "Checker.h"
#include "HashCheckerEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class HashChecker> HashCheckerRef;

		class HashChecker: public Checker
		{
		public:
			HashChecker(const ci::Rectf& rect, const IconPair& icons);
			virtual void mouseUp(ci::app::MouseEvent &event);
		};
	}
}