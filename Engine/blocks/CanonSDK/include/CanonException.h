#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Exception.h"

namespace canon
{
	class CanonException : public ci::Exception
	{
	public:
		const char* what() const throw();
	protected:
		char			mMessage[2048];
		friend class	CinderCanon;
	};

	//! Exception representing failure to initialize device.
	class ExcInitializeSDK : public CanonException 
	{
	public:
		ExcInitializeSDK() throw();
	};	

	class ExcCouldnotGetCameraList : public CanonException 
	{
	public:
		ExcCouldnotGetCameraList() throw();
	};

	class ExcCameraListEmpty : public CanonException 
	{
	public:
		ExcCameraListEmpty() throw();
	};

	class ExcCouldnotGetCamera : public CanonException 
	{
	public:
		ExcCouldnotGetCamera() throw();
	};

	class ExcCouldnotOpenSession : public CanonException 
	{
	public:
		ExcCouldnotOpenSession() throw();
	};

	class ExcTakenPhoto : public CanonException 
	{
	public:
		ExcTakenPhoto(std::string err) throw();
	};

	class ExcStartLiveView : public CanonException 
	{
	public:
		ExcStartLiveView(std::string err) throw();
	};

	class ExcStopLiveView : public CanonException 
	{
	public:
		ExcStopLiveView(std::string err) throw();
	};

	class ExcExtendShutDownTimer : public CanonException 
	{
	public:
		ExcExtendShutDownTimer(std::string err) throw();
	};

	class ExcPhotoDownload : public CanonException 
	{
	public:
		ExcPhotoDownload(std::string err) throw();
	};

	class ExcDeviceInfo : public CanonException 
	{
	public:
		ExcDeviceInfo(std::string err) throw();
	};
}