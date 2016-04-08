#include "TimerTools.h"

using namespace kubik;

void TimerTools::delaycall(const CallbackFn& callback, float time)
{
	ci::app::timeline().apply(&animTime, 0.0f, 10.0f, time).finishFn(callback);
}

void TimerTools::delaycall(const CallbackFn& callback, float time, std::string key)
{
	ci::Anim<float> animTime;
	callbacks[key] = animTime;

	ci::app::timeline().apply(&callbacks[key], 0.0f, 10.0f, time).finishFn(callback);
}

void TimerTools::clearDelaycall()
{
	animTime.stop();

	for (auto& it : callbacks)
	{
		it.second.stop();
	}
}

void TimerTools::clearDelaycall(std::string key)
{
	callbacks[key].stop();
}
