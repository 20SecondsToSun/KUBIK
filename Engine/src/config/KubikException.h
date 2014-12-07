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

class ExcBigFileSizeOfScreenSaver : public KubikException 
{
public:
	ExcBigFileSizeOfScreenSaver() throw();
};

class ExcConfigFileParsing : public KubikException 
{
public:
	ExcConfigFileParsing() throw();
};

class ExcFileDoesNotExist : public KubikException 
{
public:
	ExcFileDoesNotExist() throw();
};

class ExcGameDoesNotExist : public KubikException 
{
public:
	ExcGameDoesNotExist() throw();
};
