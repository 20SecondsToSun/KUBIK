#pragma once
#include "gui/EventGUI.h"
#include "DesignData.h"

namespace kubik
{
	class ChangePhotoFilterPreviewActiveEvent: public EventGUI
	{			
	public:
		ChangePhotoFilterPreviewActiveEvent(OneDesignItem item):item(item)
		{

		}

		OneDesignItem getItem() const
		{
			return item;
		}

	private:
		OneDesignItem item;
	};

	typedef std::shared_ptr<ChangePhotoFilterPreviewActiveEvent> ChangePhotoFilterPreviewActiveEventRef;
}