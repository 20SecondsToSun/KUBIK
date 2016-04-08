#pragma once

#include "Types.h"
#include "ISettings.h"
#include "ScreenStorage.h"

namespace kubik
{
	typedef std::shared_ptr<class IScreen> IScreenRef;

	class IScreen
	{
	public:
		IScreen(const ScreenId& id = ScreenId::UNDEFINED);
		virtual ~IScreen() {};
	
		ScreenId getType() const;
		void setType(const ScreenId& id);

		virtual void draw();
		virtual void start();
		virtual void stop();
		virtual void init(config::ISettingsRef config) = 0;
		virtual void reset() = 0;		
		void setScrenshot();

		SignalVoid closeLocationSignal;	

	protected:
		ScreenId id;
	};	
}