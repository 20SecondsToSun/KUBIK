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

		void setActive(bool value)
		{
			isActive = value;
		}

	private:
		bool isActive;
	};

	typedef std::shared_ptr<CheckerEvent> CheckerEventRef;
}