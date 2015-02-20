#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class GameCheckerEvent: public EventGUI
	{			
	public:
		GameCheckerEvent(bool isActive, int gameID):isActive(isActive), gameID(gameID)
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

		int getGameId()
		{
			return gameID;
		}

		void setActive(bool value)
		{
			isActive = value;
		}

	private:
		bool isActive;
		int gameID;

	};

	typedef std::shared_ptr<GameCheckerEvent> GameCheckerEventRef;
}