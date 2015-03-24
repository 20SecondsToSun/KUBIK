#include "main/gamesBlock/GameChecker.h"

using namespace kubik;
using namespace kubik::config;

GameChecker::GameChecker(const Rectf& rect, const GamesInfo& info, const Color& activeColor, const Color& unActiveColor)
	:Checker(rect, info.getIcons(), activeColor, unActiveColor), info(info)
{
	setActive(info.isGameOn());
	setBorderColorUnActive(ci::Color::hex(0x191425));
	setBorderColorActive(ci::Color::hex(0x242135));
	event = GameCheckerEventRef(new GameCheckerEvent(isActive, info.getGameId()));
}

void GameChecker::mouseUp(ci::app::MouseEvent &_event)
{
	if (inButtonField(_event.getPos()))
	{
		swapActive();
		GameCheckerEventRef eventref = static_pointer_cast<GameCheckerEvent>(event);
		eventref->setActive(isActive);
		event = eventref;
		Sprite::mouseUp(_event);
	}
}