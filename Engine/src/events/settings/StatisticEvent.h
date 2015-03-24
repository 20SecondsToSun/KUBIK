#pragma once
#include "gui/EventGUI.h"
#include "Types.h"

namespace kubik
{
	class StatisticEvent: public EventGUI
	{			
	public:
		StatisticEvent(GameId gameId):gameId(gameId)
		{

		}

		GameId getGameId()
		{
			return gameId;
		}

	private:
		GameId gameId;
	};

	typedef std::shared_ptr<StatisticEvent> StatisticEventRef;
}