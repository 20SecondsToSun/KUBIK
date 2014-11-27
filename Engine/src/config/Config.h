#pragma once
#include "cinder/app/AppNative.h"
#include <boost/algorithm/string.hpp>
#include "cinder/Json.h"

#include "ApplicationModel.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class Config
{
public:

	static Config& getInstance() { static Config conf; return conf; };

	void load(ApplicationModel *model)
	{
		fs::path basePath = getAppPath();

		try	
		{
			JsonTree configJSON = JsonTree(loadFile(basePath / "data/config.txt"));
			model->setUserID( configJSON.getChild( "userID" ).getValue<string>() );
		}
		catch(...)
		{
			errorHandler();
			return;
		}

		if (completeHandler)
			completeHandler();
	}

	void addCompleteListener(const std::function<void(void)>& handler)
	{
		completeHandler = handler;
	}

	void addErrorListener(const std::function<void(void)>& handler)
	{
		errorHandler = handler;
	}

private:
	std::function<void(void)> completeHandler;
	std::function<void(void)> errorHandler;
};

inline Config&	config() { return Config::getInstance(); };