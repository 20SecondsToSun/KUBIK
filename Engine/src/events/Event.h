#pragma once

using namespace ci::signals;
using namespace ci::app;
using namespace std;
using namespace ci;

namespace kubik
{
	class Event
	{
	public:
		Event(string event = ""):event(event)
		{

		}
		virtual string getMsg()
		{
			return event;
		}

	protected:
		string event;
	};

	typedef shared_ptr<Event> EventRef;	
}