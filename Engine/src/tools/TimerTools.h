#pragma once

namespace kubik
{
	class TimerTools
	{
	public:
		typedef std::function<void()> CallbackFn;

		static TimerTools& getInstance()
		{
			static TimerTools timertools;
			return timertools;
		};

		void delaycall(const CallbackFn& callback, float time);
		void delaycall(const CallbackFn& callback, float time, std::string key);
		void clearDelaycall();
		void clearDelaycall(std::string key);

	private:	
		ci::Anim<float> animTime;
		std::map<std::string, ci::Anim<float>> callbacks;
	}; 

	inline TimerTools& timertool()
	{
		return TimerTools::getInstance(); 
	};

	inline void delaycall(const TimerTools::CallbackFn& callback, float time) 
	{
		TimerTools::getInstance().delaycall(callback, time); 
	};

	inline void delaycall(const TimerTools::CallbackFn& callback, float time, std::string key) 
	{
		TimerTools::getInstance().delaycall(callback, time, key); 
	};

	inline void clearDelaycall()
	{
		TimerTools::getInstance().clearDelaycall();
	};

	inline void clearDelaycall(std::string key)
	{
		TimerTools::getInstance().clearDelaycall(key); 
	};
}
