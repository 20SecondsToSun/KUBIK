#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "ApplicationModel.h"
#include "ServiceMessage.h"
#include "IConfig.h"
#include "Types.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class GameConfigs: public IConfig
{

public:
	PhotoboothSettings  *photoBoothSettings;
	FuncesSettings  *funcesSettings;
	ApplicationModel  *model;

	bool isError;

	void load(ApplicationModel *model)
	{
		isError = false;

		this->model = model;

		vector<int> gameIDs = model-> getGameIDsTurnOn();

		for (auto gameID: gameIDs)
		{

			switch (gameID)
			{
				case gameId::PHOTOBOOTH:
					parsePhotoboothSettings();
				break;

				case gameId::FUNCES:
					parseFuncesSettings();
				break;

				default:
				break;
			}

			if(isError)
			{
				ServiceMessage msg(100);
				errorHandler(msg);
				return;
			}
		}

		console()<<"__set ::: "<<photoBoothSettings<<endl;

		if (completeHandler)
			completeHandler();
	}

private:
	void parsePhotoboothSettings()
	{
		console()<<"parse photobooth settings"<<endl;

		photoBoothSettings = new PhotoboothSettings();
		

   //"filtersIds"    : [1, 2],
   //"photoNum": 5,
   //"isFacebook": true,
   //"isVkotakte": true,
   //"isTwitter": true,
   //"isEmail": true,
   //"isQrCode": true,
   //"isPrint": true  

		try	
		{
			JsonTree configJSON = JsonTree(loadFile(getSettingsConfigPath()));
			photoBoothSettings->setSeconds(configJSON.getChild("seconds").getValue<int>());
			photoBoothSettings->setSecondsBetweenShots(configJSON.getChild("secondsBetweenShots").getValue<int>());
			
			//photoBoothSettings->
			/*vector<int> temp;
			
			JsonTree gamesAvailable = JsonTree(configJSON.getChild( "gamesAvailable"));
			for(auto it = gamesAvailable.begin(); it != gamesAvailable.end(); ++it)
				temp.push_back(it->getValue<int>());

			temp.clear();*/
			console()<<"set ::: "<<photoBoothSettings<<endl;
			model->setPhotoboothSettings(photoBoothSettings);
		}
		catch(...)
		{
			isError = true;
			return;
		}

	}

	void parseFuncesSettings()
	{
		console()<<"parse funces settings"<<endl;
	}

	fs::path getSettingsConfigPath()
	{
		fs::path basePath = getAppPath();	
		return basePath / "data" / "configs" / "photobooth.txt";
	}
};