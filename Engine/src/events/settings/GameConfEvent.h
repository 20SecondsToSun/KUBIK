#pragma once
#include "Event.h"

namespace kubik
{
	class GameConfEvent: public Event
	{			
	public:
		GameConfEvent(GameId gameId):gameId(gameId)
		{

		}

		GameId getGameId()
		{
			return gameId;
		}

	private:
		GameId gameId;
	};

	typedef std::shared_ptr<GameConfEvent> GameConfEventRef;
}