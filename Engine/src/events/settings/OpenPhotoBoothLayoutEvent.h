#pragma once
#include "Event.h"

using namespace ci::signals;
using namespace ci::app;
using namespace std;
using namespace ci;

namespace kubik
{
	class OpenPhotoBoothLayoutEvent: public Event
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