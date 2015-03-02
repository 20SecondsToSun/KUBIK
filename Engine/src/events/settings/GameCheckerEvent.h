#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class GameCheckerEvent: public EventGUI
	{			
	public:
		GameCheckerEvent(bool isActive, GameId gameID):isActive(isActive), gameID(gameID)
		{
			//console()<<"new checker event----------------"<<endl;
		}

		~GameCheckerEvent()
		{
			//console()<<"delete checker event----------------"<<endl;
		}

		bool getValue()
		{
			return isActive;
		}

		GameId getGameId()
		{
			return gameID;
		}

		void setActive(bool value)
		{
			isActive = value;
		}

	private:
		bool isActive;
		GameId gameID;

	};

	typedef std::shared_ptr<GameCheckerEvent> GameCheckerEventRef;
}