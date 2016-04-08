#pragma once

#include "gui/EventGUI.h"

namespace kubik
{
	typedef std::shared_ptr<class OpenSystemDirectoryEvent> OpenSystemDirectoryEventRef;

	class OpenSystemDirectoryEvent: public EventGUI
	{
	public:
		OpenSystemDirectoryEvent(const std::string& path = ""):path(path)
		{

		}

		std::string getPath() const
		{
			return path;
		}

	private:
		std::string path;
	};	
}