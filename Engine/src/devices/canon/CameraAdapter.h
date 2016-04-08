#pragma once

#include "CameraCanon.h"

class CameraAdapter :public canon::CameraCanon
{
public:	
	static CameraAdapter* Instance() 
	{
		return &CameraAdapterState;
	}

private:
	static CameraAdapter CameraAdapterState;

};
inline CameraAdapter&	cameraCanon() {return *CameraAdapter::Instance();};