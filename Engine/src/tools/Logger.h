#pragma once

namespace kubik
{	
	class Logger
	{
		static bool init;
		static boost::filesystem::path logPath;

	public:
		static Logger& getInstance();
		void log(const std::string & message);
		void log(const std::string & message, const std::string& filename);

	private:
		static std::string fix2(const std::string & strtofix);
		
	};
	
	inline Logger& logger() 
	{
		return Logger::getInstance();
	};
}
