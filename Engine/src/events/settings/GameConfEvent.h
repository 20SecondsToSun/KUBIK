#pragma once
#include "gui/EventGUI.h"


namespace kubik
{
	class GameConfEvent: public EventGUI
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