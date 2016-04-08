#pragma once

#include "gui/EventGUI.h"

namespace kubik
{
	class CloseActivityEvent: public EventGUI
	{			
	public:
		CloseActivityEvent()
		{

		}
	};

	typedef std::shared_ptr<CloseActivityEvent> CloseActivityEventRef;
}