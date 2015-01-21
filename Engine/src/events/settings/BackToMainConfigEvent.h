#pragma once
#include "Event.h"

namespace kubik
{
	class BackToMainConfigEvent: public Event
	{			
	public:
		BackToMainConfigEvent( )
		{

		}
	};

	typedef std::shared_ptr<BackToMainConfigEvent> BackToMainConfigEventRef;
}