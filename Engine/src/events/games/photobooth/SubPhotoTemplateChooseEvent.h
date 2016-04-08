#pragma once

#include "gui/EventGUI.h"

namespace kubik
{
	class SubPhotoTemplateChooseEvent: public EventGUI
	{			
	public:
		SubPhotoTemplateChooseEvent(int templateID):templateID(templateID)
		{

		}

		int getTemplateID() const
		{
			return templateID;
		}

	private:
		int templateID;
	};

	typedef std::shared_ptr<SubPhotoTemplateChooseEvent> SubPhotoTemplateChooseEventRef;
}