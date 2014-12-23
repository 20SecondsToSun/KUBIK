#pragma once 

#include "CameraEventListener.h"

using namespace canon;

EdsError EDSCALLBACK CameraEventListener::handleObjectEvent(EdsUInt32 inEvent, EdsBaseRef inRef, EdsVoid *inContext)
{
	CameraController *controller = (CameraController*)inContext;	

	switch(inEvent)
	{
	case kEdsObjectEvent_DirItemRequestTransfer:
	case kEdsObjectEvent_DirItemCreated:			
		EdsDirectoryItemRef dirItem = (EdsDirectoryItemRef)inRef;
		controller->downloadImage(dirItem);
		break;	
	}

	if(inRef != NULL) 
		EdsRelease(inRef);	

	return EDS_ERR_OK;
}

EdsError EDSCALLBACK CameraEventListener::handlePropertyEvent( EdsUInt32 inEvent, EdsUInt32 inPropertyID, EdsUInt32 inParam, EdsVoid* inContext)
{
	//console()<<"Cinder-Canon: handlePropertyEvent "<< CanonEventToString(inEvent)<<std::endl;
	CameraController *controller = (CameraController*)inContext;

	if(inPropertyID == kEdsPropID_Evf_OutputDevice)
		handleEvent(controller, READY_LIVE_VIEW);

	return EDS_ERR_OK;
}

EdsError EDSCALLBACK CameraEventListener::handleStateEvent(EdsUInt32 inEvent, EdsUInt32 inParam, EdsVoid* inContext)
{
	//console()<<"Cinder-Canon: handleStateEvent "<< CanonEventToString(inEvent)<<std::endl;
	CameraController *controller = (CameraController*)inContext;

	switch(inEvent)
	{
	case kEdsStateEvent_Shutdown:
		handleEvent(controller, CAMERA_SHUTDOWN);
		break;

	case kEdsStateEvent_WillSoonShutDown:
		handleEvent(controller, CAMERA_WILL_SOON_SHUTDOWN);
		break;	
	};

	return EDS_ERR_OK;
}

void CameraEventListener::handleEvent(CameraController *controller, canonEvent command, void* arg)
{
	controller->perform(command);
}
