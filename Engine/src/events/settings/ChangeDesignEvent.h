#pragma once

#include "gui/EventGUI.h"
#include "DesignData.h"

namespace kubik
{
	class ChangeDesignEvent: public EventGUI
	{			
	public:
		ChangeDesignEvent(config::OneDesignItem item) :item(item)
		{

		}

		config::OneDesignItem getItem() const
		{
			return item;
		}

	private:
		config::OneDesignItem item;
	};

	typedef std::shared_ptr<ChangeDesignEvent> ChangeDesignEventRef;
}