#pragma once

#include "gui/EventGUI.h"

namespace kubik
{
	class StartNewActivityEvent: public EventGUI
	{			
	public:
		StartNewActivityEvent()
		{

		}
	};

	typedef std::shared_ptr<StartNewActivityEvent> StartNewActivityEventRef;
}