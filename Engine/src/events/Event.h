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
		Event(const std::string& event = ""):event(event)
		{

		}
		virtual string getMsg()
		{
			return event;
		}

	protected:
		std::string event;
	};

	typedef std::shared_ptr<Event> EventRef;
}