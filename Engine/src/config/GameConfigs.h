#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "ApplicationModel.h"
#include "KubikException.h"
#include "IConfig.h"
#include "Types.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class GameConfigs: public IConfig
	{

	public:	

		void load(ApplicationModel *model, GameSettings *gameSettings)
		{		
			this->model = model;

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
		PhotoboothSettings  *photoBoothSettings;
		FuncesSettings  *funcesSettings;
		ApplicationModel  *model;
		GameSettings *gameSettings;	

		
	};
}