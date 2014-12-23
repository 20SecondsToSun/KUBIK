#pragma once 

#include "cinder/app/AppBasic.h"
#include "EDSDK.h"
#include "CanonEventCodes.h"
#include "CameraController.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace ci::signals;

namespace canon
{
	class CameraEventListener
	{
	public:
		static EdsError EDSCALLBACK handleObjectEvent(EdsUInt32 inEvent, EdsBaseRef inRef, EdsVoid *inContext);
		static EdsError EDSCALLBACK handlePropertyEvent(EdsUInt32 inEvent, EdsUInt32 inPropertyID, EdsUInt32 inParam, EdsVoid* inContext);
		static EdsError EDSCALLBACK handleStateEvent(EdsUInt32 inEvent, EdsUInt32 inParam, EdsVoid* inContext);

	private:
		static void handleEvent(CameraController *controller, canonEvent command, void* arg = 0);
	};
}