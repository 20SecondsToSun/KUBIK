#pragma once

#include "gui/EventGUI.h"

namespace kubik
{
	class CloseConfigEvent: public EventGUI
	{			
	public:
		CloseConfigEvent()
		{

		}
	};

	typedef std::shared_ptr<CloseConfigEvent> CloseConfigEventRef;
}