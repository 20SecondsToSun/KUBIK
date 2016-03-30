#include "Logger.h"
#include "Utils.h"

using namespace kubik;

bool Logger::init = false;
std::string Logger::logName = "";

Logger& Logger::getInstance()
{
	static Logger logger;

	if (!init)
	{
		auto timeStruct = Utils::getCurrentTime();
		logName = to_string(1900 + timeStruct.tm_year) + "_"
				+ to_string(timeStruct.tm_mon+1) + "_"
				+ to_string(timeStruct.tm_mday) + "-"
				+ to_string(timeStruct.tm_hour) + "_"
				+ to_string(timeStruct.tm_min) + "_"
				+ to_string(timeStruct.tm_sec) +
				".log";
		auto path = ci::app::getAppPath() /"logs"/ logName;	

		ci::app::console()<< "path to save " << path << std::endl;

		std::ofstream logfile;		
		logfile.open(path.c_str());
		logfile.close();

		init = true;
	}

	return logger;
};

void Logger::log(const std::string & message)
{
	std::ofstream logfile;
	auto path = ci::app::getAppPath() / logName;
	logfile.open(path.c_str(), ios::out | ios::app);
	logfile << message << std::endl;
	logfile.close();
	
#ifdef CINDER_MSW
	OutputDebugStringA((message + '\n').c_str());
#endif

}