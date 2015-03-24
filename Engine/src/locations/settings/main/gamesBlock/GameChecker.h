#pragma once
#include "Checker.h"
#include "CheckerEvent.h"
#include "GameCheckerEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class GameChecker> GameCheckerRef;

		class GameChecker: public Checker
		{
		public:
			GameChecker(const ci::Rectf& rect, const GamesInfo& info, const ci::Color& activeColor = ci::Color::hex(0x00b6c4), const ci::Color& unActiveColor = ci::Color::hex(0x08060f));
			virtual void mouseUp(ci::app::MouseEvent &_event);

		private:
			GamesInfo info;
		};
	}
}