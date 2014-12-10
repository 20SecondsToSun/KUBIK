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

		GameSettings(shared_ptr<ApplicationModel> model)
		{
			this->model = model;
			currentGame = model->getDefaultGameID();

			load();
		}

		shared_ptr<ISettings> get(int id)
		{
			return gameSettingsMap[id];
		}		

		Types::OneBlockTexDictionary getActiveGameTextures()
		{
			return getGameTexturesById(currentGame);
		}

		Types::OneBlockTexDictionary getGameTexturesById(int id)
		{
			return gameSettingsMap[id]->getTextures();		
		}		

		void setTextures() override
		{
			gameSettingsMap[currentGame]->setTextures();	
		}

		void load() override
		{				
			vector<int> gameIDs = model->getGameIDsTurnOn();

			for (auto gameID: gameIDs)
			{
				switch (gameID)
				{
				case gameId::PHOTOBOOTH:					
					gameSettingsMap[gameId::PHOTOBOOTH] = shared_ptr<PhotoboothSettings>(new PhotoboothSettings(model));				
					break;

				case gameId::FUNCES:
					gameSettingsMap[gameId::FUNCES]  = shared_ptr<FuncesSettings>(new FuncesSettings(model));	
					break;

				default:
					continue;
				}

				try	
				{				
					gameSettingsMap[gameID]->load();
				}
				catch(...)
				{
					throw ExcConfigFileParsing();
				}
			}
		}
	
		bool isGameID(int id)
		{
			vector<int> gameIDs = model->getGameIDsTurnOn();
			for (int value: gameIDs)
			{
				if(value == id)
					return true;
			}
			return false;
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

		void setNextGameId(int id)
		{
			nextGameId = id;
		}

		int getNextGameId()
		{
			return nextGameId;
		}		

	private:

		int currentGame, nextGameId;	
		map<int, shared_ptr<ISettings>> gameSettingsMap;
	};
}