#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class PrinterControlsHideEvent: public EventGUI
	{			
	public:
		PrinterControlsHideEvent()
		{

		}
	};

	typedef std::shared_ptr<PrinterControlsHideEvent> PrinterControlsHideEventRef;
}