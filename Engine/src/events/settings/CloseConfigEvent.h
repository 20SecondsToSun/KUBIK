#pragma once
#include "Event.h"

namespace kubik
{
	class CloseConfigEvent: public Event
	{			
	public:
		CloseConfigEvent( )
		{

		}
	};

	typedef std::shared_ptr<CloseConfigEvent> CloseConfigEventRef;
}