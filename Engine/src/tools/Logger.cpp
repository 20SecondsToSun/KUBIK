#include "Logger.h"
#include "Utils.h"

using namespace kubik;

bool Logger::init = false;
boost::filesystem::path Logger::logPath = "";

Logger& Logger::getInstance()
{
	static Logger logger;

	if (!init)
	{
		auto timeStruct = Utils::getCurrentTime();
		auto logName = to_string(1900 + timeStruct.tm_year) + "_"
				+ to_string(timeStruct.tm_mon+1) + "_"
				+ to_string(timeStruct.tm_mday) + "-"
				+ to_string(timeStruct.tm_hour) + "_"
				+ to_string(timeStruct.tm_min) + "_"
				+ to_string(timeStruct.tm_sec) +
				".log";

		logPath = ci::app::getAppPath() / "logs" / logName;

		std::ofstream logfile;		
		logfile.open(logPath.c_str());
		logfile.close();

		init = true;
	}

	return logger;
};

void Logger::log(const std::string & message)
{
	std::ofstream logfile;
	logfile.open(logPath.c_str(), ios::out | ios::app);
	logfile << message << std::endl;
	logfile.close();
	
#ifdef CINDER_MSW
	OutputDebugStringA((message + '\n').c_str());
#endif

}