#pragma once

namespace kubik
{
	
	class Logger
	{
	public:

		static Logger& getInstance() { 
			static Logger logger; 
			return logger; 
		};	

		void log(const std::string & message)
		{
			// Write to console and debug window
			//ci::app::console() << message << std::endl;
			#ifdef CINDER_MSW
				OutputDebugStringA((message + '\n').c_str());
			#endif
		}
	};
	// helper function(s) for easier access 
	inline Logger& logger() {return Logger::getInstance();};
}