#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class PhotoboothSettings:public ISettings
{
public:

	void setSeconds(int _seconds)
	{
		seconds = _seconds;
	}

	int getSeconds()
	{
		return seconds;
	}
	
	void setSecondsBetweenShots(int _secondsBetweenShots)
	{
		secondsBetweenShots = _secondsBetweenShots;
	}

private:

	int secondsBetweenShots;
	int seconds;
};