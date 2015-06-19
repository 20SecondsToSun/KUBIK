#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class CloseDesignLayoutEvent: public EventGUI
	{			
	public:
		CloseDesignLayoutEvent()
		{

		}
	};

	typedef std::shared_ptr<CloseDesignLayoutEvent> CloseDesignLayoutEventRef;
}