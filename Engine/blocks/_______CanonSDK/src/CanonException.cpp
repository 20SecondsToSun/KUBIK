#include "CanonException.h"

using namespace canon;

const char* CanonException::what() const throw() 
{ 
	return mMessage; 
}

ExcInitializeSDK::ExcInitializeSDK() throw()
{
	sprintf_s(mMessage, "Cinder-Canon :: Couldn't initialize SDK");	
}

ExcCouldnotGetCameraList::ExcCouldnotGetCameraList() throw()
{
	sprintf_s(mMessage, "Cinder-Canon :: Couldn't retrieve camera list");	 
}

ExcCameraListEmpty::ExcCameraListEmpty() throw()
{
	sprintf_s(mMessage, "Cinder-Canon :: camera list is empty");	
}

ExcCouldnotGetCamera::ExcCouldnotGetCamera() throw()
{
	sprintf_s(mMessage, "Cinder-Canon :: Couldn't retrieve camera from list");	
}

ExcCouldnotOpenSession::ExcCouldnotOpenSession() throw()
{
	sprintf_s(mMessage, "Cinder-Canon :: Couldn't open camera session");	
}

ExcTakenPhoto::ExcTakenPhoto(std::string err) throw()
{	
	sprintf_s(mMessage, "Cinder-Canon :: take picture error :: ");	
}

ExcStartLiveView::ExcStartLiveView(std::string err) throw()
{	
	sprintf_s(mMessage, "Cinder-Canon :: start live view error :: ");
}

ExcStopLiveView::ExcStopLiveView(std::string err) throw()
{	
	sprintf_s(mMessage, "Cinder-Canon :: stop live view error :: ");
}

ExcExtendShutDownTimer::ExcExtendShutDownTimer(std::string err) throw()
{	
	sprintf_s(mMessage, "Cinder-Canon :: stop live view error :: ");
}

ExcPhotoDownload::ExcPhotoDownload(std::string err) throw()
{	
	sprintf_s(mMessage, "Cinder-Canon :: photo download error :: ");
}

ExcDeviceInfo::ExcDeviceInfo(std::string err) throw()
{	
	sprintf_s(mMessage, "Cinder-Canon :: device info error :: ");
}
