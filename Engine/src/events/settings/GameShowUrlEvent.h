#pragma once
#include "Event.h"

namespace kubik
{
	class GameShowUrlEvent: public Event
	{			
	public:
		GameShowUrlEvent(GameId gameId):gameId(gameId)
		{

		}

		GameId getGameId()
		{
			return gameId;
		}

	private:
		GameId gameId;
	};

	typedef std::shared_ptr<GameShowUrlEvent> GameShowUrlEventRef;
}