#pragma once
#include "Types.h"

namespace kubik
{
	class ILoader
	{
	public:
		SignalVoid LoadingCompleteSignal;
		SignalException LoadingErrorSignal;	
		virtual void load() = 0;	
	};
}