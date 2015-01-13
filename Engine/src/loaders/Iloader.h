#pragma once

#include "Types.h"

using namespace std;
using namespace ci;

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