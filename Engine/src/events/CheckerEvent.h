#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class CheckerEvent: public EventGUI
	{			
	public:
		CheckerEvent(bool isActive):isActive(isActive)
		{

		}

		bool getValue()
		{
			return isActive;
		}

	private:
		bool isActive;
	};

	typedef std::shared_ptr<CheckerEvent> CheckerEventRef;
}