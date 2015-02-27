#pragma once
#include "gui/EventGUI.h"
#include "DesignData.h"

namespace kubik
{
	class PhotoFilterEvent: public EventGUI
	{			
	public:
		PhotoFilterEvent(int filterId):filterId(filterId)
		{

		}

		int getFilterId()
		{
			return filterId;
		}

	private:
		int filterId;
	};

	typedef std::shared_ptr<PhotoFilterEvent> PhotoFilterEventRef;
}