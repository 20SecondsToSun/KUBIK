#pragma once
#include "Event.h"

namespace kubik
{
	class SavePhotobootnConfigEvent: public Event
	{			
	public:
		SavePhotobootnConfigEvent( )
		{

		}
	};

	typedef std::shared_ptr<SavePhotobootnConfigEvent> SavePhotobootnConfigEventRef;
}