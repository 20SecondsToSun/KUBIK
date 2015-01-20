#pragma once
#include "Event.h"

namespace kubik
{
	class StatisticEvent: public Event
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