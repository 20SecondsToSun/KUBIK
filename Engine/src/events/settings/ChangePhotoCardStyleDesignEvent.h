#pragma once

#include "gui/EventGUI.h"
#include "DesignData.h"

namespace kubik
{
	class ChangePhotoCardStyleDesignEvent: public EventGUI
	{			
	public:
		ChangePhotoCardStyleDesignEvent(OneDesignItem item):item(item)
		{

		}

		OneDesignItem getItem() const
		{
			return item;
		}

	private:
		OneDesignItem item;
	};

	typedef std::shared_ptr<ChangePhotoCardStyleDesignEvent> ChangePhotoCardStyleDesignEventRef;
}