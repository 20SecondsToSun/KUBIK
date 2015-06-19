#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class PhotoChoosedEvent: public EventGUI
	{			
	public:
		PhotoChoosedEvent(int ID):ID(ID)
		{

		}

		int getID() const
		{
			return ID;
		}

	private:
		int ID;
	};

	typedef std::shared_ptr<PhotoChoosedEvent> PhotoChoosedEventRef;
}