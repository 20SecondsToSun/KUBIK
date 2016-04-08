#pragma once

#include "gui/EventGUI.h"

namespace kubik
{
	class FilterChangedEvent: public EventGUI
	{			
	public:
		FilterChangedEvent(int ID):ID(ID)
		{

		}

		int getID() const
		{
			return ID;
		}

	private:
		int ID;
	};

	typedef std::shared_ptr<FilterChangedEvent> FilterChangedEventRef;
}