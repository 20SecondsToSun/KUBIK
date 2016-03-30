#pragma once

namespace kubik
{	
	class Logger
	{
		static bool init;
		static std::string logName;

	public:
		static Logger& getInstance();
		void log(const std::string & message);
		
	};
	
	inline Logger& logger() {return Logger::getInstance();};
}