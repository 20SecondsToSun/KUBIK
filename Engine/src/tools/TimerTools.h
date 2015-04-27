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

		void delaycall(const CallbackFn& callback, float time, int key)
		{
			ci::Anim<float> animTime;
			callbacks[key] = animTime;

			ci::app::timeline().apply(&callbacks[key], 0.0f, 10.0f, time).finishFn(callback);			
		}

		void clearDelaycall()
		{
			animTime.stop();	

			for (auto& it : callbacks)
				it.second.stop();
		}

		void clearDelaycall(int key)
		{
			callbacks[key].stop();			
		}

	private:	
		ci::Anim<float> animTime;
		std::map<int, ci::Anim<float>> callbacks;

	}; 
	// helper function(s) for easier access 
	inline TimerTools& timertool() { return TimerTools::getInstance(); };
	inline void delaycall(const TimerTools::CallbackFn& callback, float time) { TimerTools::getInstance().delaycall(callback, time); };
	inline void delaycall(const TimerTools::CallbackFn& callback, float time, int key) { TimerTools::getInstance().delaycall(callback, time, key); };
	inline void clearDelaycall() { TimerTools::getInstance().clearDelaycall(); };
	inline void clearDelaycall(int key) { TimerTools::getInstance().clearDelaycall(key); };
}