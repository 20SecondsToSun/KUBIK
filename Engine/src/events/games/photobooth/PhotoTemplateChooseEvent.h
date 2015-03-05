#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class PhotoTemplateChooseEvent: public EventGUI
	{			
	public:
		PhotoTemplateChooseEvent(int templateID):templateID(templateID)
		{

		}

		int getTemplateID()
		{
			return templateID;
		}

	private:
		int templateID;
	};

	typedef std::shared_ptr<PhotoTemplateChooseEvent> PhotoTemplateChooseEventRef;
}