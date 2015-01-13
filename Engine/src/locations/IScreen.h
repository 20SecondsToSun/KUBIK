#pragma once

#include "Types.h"
#include "ISettings.h"

using namespace ci::signals;
using namespace std;

namespace kubik
{
	class IScreen
	{

	public:
		IScreen(ScreenId id = ScreenId::UNDEFINED):id(id)
		{

		}

		~IScreen()
		{
		}

		ScreenId getType()
		{
			return id;
		}

		void setType(ScreenId id)
		{
			this->id = id;
		}

		virtual void draw() = 0;		
		virtual void start(){};		
		virtual void stop(){};		
		virtual void init(ISettingsRef config) = 0;
		virtual void reset() = 0;

		SignalVoid closeLocationSignal;

	protected:
		ScreenId id;
	};

	typedef shared_ptr<IScreen> IScreenRef;
}