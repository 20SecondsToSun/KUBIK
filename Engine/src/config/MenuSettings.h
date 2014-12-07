#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"
#include "ApplicationModel.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class MenuSettings:public ISettings
{
public:
	MenuSettings(ApplicationModel *model)
	{
	}

private:

};