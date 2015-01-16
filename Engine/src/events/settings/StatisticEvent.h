#pragma once
#include "Event.h"

using namespace ci::signals;
using namespace ci::app;
using namespace std;
using namespace ci;

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