#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class OpenPhotoBoothLayoutEvent: public EventGUI
	{			
	public:
		OpenPhotoBoothLayoutEvent(int index):index(index)
		{

		}

		int getlayoutIndex()
		{
			return index;
		}

	private:
		int index;
	};

	typedef std::shared_ptr<OpenPhotoBoothLayoutEvent> OpenPhotoBoothLayoutEventRef;
}