#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class PrinterStatResetEvent: public EventGUI
	{			
	public:
		PrinterStatResetEvent( )
		{

		}
	};

	typedef std::shared_ptr<PrinterStatResetEvent> PrinterStatResetEventRef;
}