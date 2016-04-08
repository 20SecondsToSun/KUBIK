#pragma once

#include "gui/EventGUI.h"

namespace kubik
{
	class SavePhotobootnConfigEvent: public EventGUI
	{			
	public:
		SavePhotobootnConfigEvent()
		{

		}
	};

	typedef std::shared_ptr<SavePhotobootnConfigEvent> SavePhotobootnConfigEventRef;
}