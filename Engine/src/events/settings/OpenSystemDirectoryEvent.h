#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class OpenSystemDirectoryEvent: public EventGUI
	{			
	public:
		OpenSystemDirectoryEvent()
		{

		}
	};

	typedef std::shared_ptr<OpenSystemDirectoryEvent> OpenSystemDirectoryEventRef;
}