#pragma once
#include "cinder/app/AppNative.h"
#include <boost/algorithm/string.hpp>
#include "cinder/Json.h"

#include "IModel.h"
#include "ServiceMessage.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class IConfig
{
public:

	//virtual void load(IModel *model) = 0;

	void addCompleteListener(const std::function<void(void)>& handler)
	{
		completeHandler = handler;
	}

	void addErrorListener(const std::function<void(ServiceMessage)>& handler)
	{
		errorHandler = handler;
	}

protected:
	fs::path getBasePath()
	{
		fs::path basePath = getAppPath();
		return basePath /"data/configs/";
	}

	std::function<void(void)> completeHandler;
	std::function<void(ServiceMessage)> errorHandler;	
};
