#pragma once

namespace kubik
{
	class MemoryMonitor
	{
	public:
		static MemoryMonitor& getInstance();

		void registerLoging();
		void log();

	private:
		ci::Anim<float> animTime;		
	};

	inline MemoryMonitor& memory_monitor()
	{ 
		return MemoryMonitor::getInstance(); 
	};
}