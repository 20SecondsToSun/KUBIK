#pragma once

#include "gui/EventGUI.h"

namespace kubik
{
	class GameShowUrlEvent: public EventGUI
	{			
	public:
		GameShowUrlEvent(GameId gameId):gameId(gameId)
		{

		}

		GameId getGameId() const
		{
			return gameId;
		}

	private:
		GameId gameId;
	};

	typedef std::shared_ptr<GameShowUrlEvent> GameShowUrlEventRef;
}