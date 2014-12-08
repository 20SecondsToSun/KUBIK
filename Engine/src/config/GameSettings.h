#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"
#include "FuncesSettings.h"
#include "PhotoboothSettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class GameSettings:public ISettings
	{
	public:

		GameSettings(ApplicationModel *model)
		{
			console()<<"game settings create   "<<endl;
			this->model = model;
			currentGame = model->getDefaultGameID();
		}

		bool isGameCurrent(int id)
		{
			return currentGame == id;
		}

		int getCurrentGame()
		{
			return currentGame;
		}

		void setCurrentGame(int id)
		{
			currentGame =  id;
		}

		ISettings* getGameSettingsById()
		{
			ISettings *config;

			switch (currentGame)
			{
			case gameId::PHOTOBOOTH:		
				config = photoBoothSettings;			
				break;

			case gameId::FUNCES:
				config = funcesSettings;
				break;

			default:
				break;
			}

			return config;
		}

		void setFuncesSettings(FuncesSettings* fs)
		{
			funcesSettings =  fs;
		}

		void setPhotoboothSettings(PhotoboothSettings* phs)
		{
			photoBoothSettings =  phs;
		}

		void loadGamesSettings()
		{				
			vector<int> gameIDs = model->getGameIDsTurnOn();

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
			}		
		}


	private:

		int currentGame;
		ApplicationModel *model;
		FuncesSettings *funcesSettings;
		PhotoboothSettings *photoBoothSettings;

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
			}
			catch(...)
			{
				throw ExcConfigFileParsing();
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
}