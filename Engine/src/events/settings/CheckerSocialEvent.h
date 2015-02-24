#pragma once
#include "gui/EventGUI.h"
#include "PhotoboothSettings.h"

namespace kubik
{
	class CheckerSocialEvent: public EventGUI
	{			
	public:
		CheckerSocialEvent(bool isActive, PhotoboothSettings::PhtTextID socialID):isActive(isActive), socialID(socialID)
		{
			//console()<<"new checker event----------------"<<endl;
		}

		~CheckerSocialEvent()
		{
			//console()<<"delete checker event----------------"<<endl;
		}

		bool getValue() const
		{
			return isActive;
		}

		PhotoboothSettings::PhtTextID getSocialID() const
		{
			return socialID;
		}

		void setActive(bool value)
		{
			isActive = value;
		}

	private:
		bool isActive;
		PhotoboothSettings::PhtTextID socialID;

	};

	typedef std::shared_ptr<CheckerSocialEvent> CheckerSocialEventRef;
}