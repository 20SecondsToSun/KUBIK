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

		void delaycall(const CallbackFn& callback, float time)
		{
			ci::app::timeline().apply(&animTime, 0.0f, 10.0f, time).finishFn(callback);
		}

		void clearDelaycall()
		{
			animTime.stop();
		}

	private:	
		ci::Anim<float> animTime;

	}; 
	// helper function(s) for easier access 
	inline TimerTools& timertool() { return TimerTools::getInstance(); };
	inline void delaycall(const TimerTools::CallbackFn& callback, float time) { TimerTools::getInstance().delaycall(callback, time); };
	inline void clearDelaycall() { TimerTools::getInstance().clearDelaycall(); };
}