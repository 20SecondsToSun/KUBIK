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

	//MEMORYSTATUSEX status;
	//status.dwLength = sizeof(status);
	//GlobalMemoryStatusEx(&status);	

	PROCESS_MEMORY_COUNTERS_EX pmcx = {};
	pmcx.cb = sizeof(pmcx);
	GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmcx), pmcx.cb);

	auto MB = 1048576.0;
	ci::app::console() << " ~~~ Memory usage log :::::::::" + ci::toString(pmcx.PrivateUsage / MB) <<" MB ~~~"<< std::endl;

	registerLoging();
}
