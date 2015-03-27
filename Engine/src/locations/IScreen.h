#pragma once
#include "Types.h"
#include "ISettings.h"
#include "ScreenStorage.h"

using namespace kubik::config;

namespace kubik
{
	typedef std::shared_ptr<class IScreen> IScreenRef;

	class IScreen
	{
	public:
		IScreen(const ScreenId& id = ScreenId::UNDEFINED);
	
		ScreenId getType() const;
		void setType(const ScreenId& id);

		virtual void draw();
		virtual void start();
		virtual void stop();
		virtual void init(ISettingsRef config) = 0;
		virtual void reset() = 0;		
		void setScrenshot();

		SignalVoid closeLocationSignal;	

	protected:
		ScreenId id;
	};	
}