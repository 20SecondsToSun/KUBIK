#pragma once
#include "cinder/app/AppNative.h"
#include <boost/algorithm/string.hpp>
#include "cinder/Json.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class IConfig
	{
	public:
		
		void addCompleteListener(const std::function<void(void)>& handler)
		{
			completeHandler = handler;
		}

	protected:
		fs::path getBasePath()
		{
			fs::path basePath = getAppPath();
			return basePath /"data/configs/";
		}

		std::function<void(void)> completeHandler;	
	};
}