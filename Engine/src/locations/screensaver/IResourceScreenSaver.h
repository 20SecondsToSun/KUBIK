#pragma once

namespace kubik
{
	class IResourceScreenSaver
	{
	public:	
		virtual void draw()  = 0;
		virtual void start() = 0;
		virtual void stop()  = 0;
	};

	typedef shared_ptr<IResourceScreenSaver> IResourceScreenSaverRef;	
}