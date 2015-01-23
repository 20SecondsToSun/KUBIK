#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class BackToMainConfigEvent: public EventGUI
	{			
	public:
		BackToMainConfigEvent()
		{

		}
	};

	typedef std::shared_ptr<BackToMainConfigEvent> BackToMainConfigEventRef;
}