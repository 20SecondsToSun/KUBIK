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
			GameChecker(ci::Rectf rect, GamesInfo info, ci::Color activeColor = ci::Color::hex(0x00b6c4), ci::Color unActiveColor = ci::Color::hex(0x08060f))
				:Checker(rect, info.getIcons(), activeColor, unActiveColor), info(info)
			{
				setActive(info.isGameOn());
				setBorderColorUnActive(ci::Color::hex(0x191425));
				setBorderColorActive(ci::Color::hex(0x242135));
				event = GameCheckerEventRef(new GameCheckerEvent(isActive, info.getGameId()));
			}	

			virtual void mouseUp(ci::app::MouseEvent &_event)
			{
				if(inButtonField(_event.getPos()))
				{
					swapActive();
					GameCheckerEventRef eventref = static_pointer_cast<GameCheckerEvent>(event);
					eventref->setActive(isActive);
					event = eventref;
					Sprite::mouseUp(_event);
				}
			}
		private:
			GamesInfo info;
		};
	}
}