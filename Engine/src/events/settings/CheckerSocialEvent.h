#pragma once
#include "gui/EventGUI.h"

namespace kubik
{
	class CheckerSocialEvent: public EventGUI
	{			
	public:
		CheckerSocialEvent(bool isActive, int socialID):isActive(isActive), socialID(socialID)
		{
			//console()<<"new checker event----------------"<<endl;
		}

		~CheckerSocialEvent()
		{
			//console()<<"delete checker event----------------"<<endl;
		}

		bool getValue()
		{
			return isActive;
		}

		int getSocialID()
		{
			return socialID;
		}

		void setActive(bool value)
		{
			isActive = value;
		}

	private:
		bool isActive;
		int socialID;

	};

	typedef std::shared_ptr<CheckerSocialEvent> CheckerSocialEventRef;
}