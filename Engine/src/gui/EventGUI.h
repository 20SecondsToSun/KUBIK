#pragma once

namespace kubik
{
	enum KubikEvent 
	{
		NONE
	};

	enum InteractEvent 
	{
		TOUCH_UP
	};

	class EventGUI
	{
	public:
		EventGUI(KubikEvent type = KubikEvent::NONE):type(type)
		{

		}

		virtual KubikEvent getType()
		{
			return type;
		}

		virtual void setType(KubikEvent value)
		{
			type = value;
		}

	protected:
		KubikEvent type;
	};

	typedef std::shared_ptr<EventGUI> EventGUIRef;	
}