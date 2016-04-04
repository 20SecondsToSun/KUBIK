#include "MemoryMonitor.h"
#include "TimerTools.h"
#include "Logger.h"
#include "windows.h"
#include "psapi.h"
#include <boost/timer.hpp>

using namespace kubik;

MemoryMonitor& MemoryMonitor::getInstance()
{
	static MemoryMonitor monitor;
	return monitor;
}

void MemoryMonitor::registerLoging()
{
	const float time = 10.0f;
	ci::app::timeline().apply(&animTime, 0.0f, 10.0f, time).finishFn(std::bind(&MemoryMonitor::log, this));
}

void MemoryMonitor::log()
{	
	auto logPath = ci::app::getAppPath() / "logs" / "memory.log";

	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);	

	ci::app::console() << "---------------memory_log---------------" + ci::toString(status.dwMemoryLoad) << std::endl;

	const float time = 10.0f;
	ci::app::timeline().apply(&animTime, 0.0f, 10.0f, time).finishFn(std::bind(&MemoryMonitor::log, this));
}
