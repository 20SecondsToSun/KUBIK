#pragma once

#include "gui/EventGUI.h"
#include "DesignData.h"

namespace kubik
{
	class ChangeDesignEvent: public EventGUI
	{			
	public:
		ChangeDesignEvent(OneDesignItem item):item(item)
		{

		}

		OneDesignItem getItem() const
		{
			return item;
		}

	private:
		OneDesignItem item;
	};

	typedef std::shared_ptr<ChangeDesignEvent> ChangeDesignEventRef;
}