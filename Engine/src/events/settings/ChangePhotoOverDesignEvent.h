#pragma once
#include "gui/EventGUI.h"
#include "DesignData.h"

namespace kubik
{
	class ChangePhotoOverDesignEvent: public EventGUI
	{			
	public:
		ChangePhotoOverDesignEvent(OneDesignItem item):item(item)
		{

		}

		OneDesignItem getItem() const
		{
			return item;
		}

	private:
		OneDesignItem item;
	};

	typedef std::shared_ptr<ChangePhotoOverDesignEvent> ChangePhotoOverDesignEventRef;
}