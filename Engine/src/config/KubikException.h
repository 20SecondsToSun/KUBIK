#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Exception.h"

class KubikException : public cinder::Exception
{
public:
	const char* what() const throw();
protected:
	char							mMessage[ 2048 ];
	bool							isCritical;
	//friend class					FaceTracker;
};

//! Exception representing failure to create FaceTracker.
class ExcBigFileSizeOfScreenSaver : public KubikException 
{
public:
	ExcBigFileSizeOfScreenSaver() throw();
};

//! Exception representing failure to create image buffer.
class ExcConfigFileParsing : public KubikException 
{
public:
	ExcConfigFileParsing() throw();
};

//! Exception representing failure to create image buffer.
class ExcFileDoesNotExist : public KubikException 
{
public:
	ExcFileDoesNotExist() throw();
};



////! Exception representing failure to create FaceTracker result.
//class ExcFaceTrackerCreateResult : public KubikException 
//{
//public:
//	ExcFaceTrackerCreateResult( long hr ) throw();
//};
//
////! Exception representing failure to initialize FaceTracker.
//class ExcFaceTrackerInit : public KubikException 
//{
//public:
//	ExcFaceTrackerInit( long hr ) throw();
//};

