#pragma once
#include "Event.h"

using namespace ci::signals;
using namespace ci::app;
using namespace std;
using namespace ci;

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