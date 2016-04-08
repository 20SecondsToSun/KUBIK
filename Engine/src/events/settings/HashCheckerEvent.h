#pragma once

#include "gui/EventGUI.h"

namespace kubik
{
	class HashCheckerEvent: public EventGUI
	{			
	public:
		HashCheckerEvent(bool isActive):isActive(isActive)
		{

		}

		virtual ~HashCheckerEvent()
		{

		}

		bool getValue() const
		{
			return isActive;
		}

		void setActive(bool value)
		{
			isActive = value;
		}

	private:
		bool isActive;
		int socialID;
	};

	typedef std::shared_ptr<HashCheckerEvent> HashCheckerEventRef;
}