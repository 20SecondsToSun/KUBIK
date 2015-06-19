#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class GameChoosedEvent: public EventGUI
	{			
	public:
		GameChoosedEvent(const GameId& gameID):gameID(gameID)
		{

		}

		GameId getGameID() const
		{
			return gameID;
		}

	private:
		GameId gameID;
	};

	typedef std::shared_ptr<GameChoosedEvent> GameChoosedEventRef;
}