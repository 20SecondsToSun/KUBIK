#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class FuncesSettings: public ISettings
	{
	public:
		void load(string path)
		{
			JsonTree configJSON = JsonTree(loadFile(path));

			designPath = configJSON.getChild("designPath").getValue<string>();
			/*seconds = configJSON.getChild("seconds").getValue<int>();
			secondsBetweenShots = configJSON.getChild("secondsBetweenShots").getValue<int>();
			photoNum			= configJSON.getChild("photoNum").getValue<int>();

			isFacebook			= configJSON.getChild("isFacebook").getValue<bool>();
			isVkotakte			= configJSON.getChild("isVkotakte").getValue<bool>();
			isTwitter			= configJSON.getChild("isTwitter").getValue<bool>();
			isEmail				= configJSON.getChild("isEmail").getValue<bool>();
			isQrCode			= configJSON.getChild("isQrCode").getValue<bool>();
			isPrint			    = configJSON.getChild("isPrint").getValue<bool>();*/
			//"filtersIds"    : [1, 2],
			setTextures();
		}

		void setTextures()
		{		
			designTexures.clear();
			addToDictionary("closeImg", getDesignPath() + "close.png");
		}
	};
}