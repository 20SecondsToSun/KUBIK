#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class PhotoTemplateChooseEvent: public EventGUI
	{			
	public:
		PhotoTemplateChooseEvent(int count):count(count)
		{

		}

		int getCount()
		{
			return count;
		}

	private:
		int count;
	};

	typedef std::shared_ptr<PhotoTemplateChooseEvent> PhotoTemplateChooseEventRef;
}