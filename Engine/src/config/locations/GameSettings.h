#pragma once
#include "cinder/app/AppNative.h"
#include "ISettings.h"
#include "FuncesSettings.h"
#include "PhotoboothSettings.h"
#include "Types.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class GameSettings:public ISettings
	{
	public:

		struct GamesDataStruct
		{
			vector<GamesInfo> games;
			int defaultGameID;	

			int getCountSwitchOnGames()
			{
				int count = 0;

				for (auto game: games)
				{
					if(game.isOn)
						count++;
				}

				return count;
			}

			bool isIdInSwitchOnGames(game::id id)
			{
				for (auto game: games)
				{
					if(game.isOn)
					{
						if(game.id == id)
							return true;
					}
				}

				return false;
			}
		};

		GameSettings(shared_ptr<ApplicationModel> model)
		{
			this->model		   = model;
			currentGame		   = model->getDefaultGameID();
			data.games		   = model->getGames();
			data.defaultGameID = model->getDefaultGameID();
			console()<<"loaded game::  "<<data.games.size()<<endl;
			load();
		}

		shared_ptr<ISettings> get(game::id id)
		{
			return gameSettingsMap[id];
		}		

		IResourceDictionary getActiveGameResources()
		{
			return getGameTexturesById(currentGame);
		}

		IResourceDictionary getGameTexturesById(game::id id)
		{
			IResourceDictionary rd = gameSettingsMap[id]->getResources();
			return rd;		
		}		

		void setTextures() override
		{
			gameSettingsMap[currentGame]->setTextures();	
		}

		void load() override
		{
			vector<GamesInfo> games = model->getGames();

			for (auto game: games)
			{
				if (!game.isOn || !game.isPurchased)
					continue;

				switch (game.id)
				{
				case  game::id::PHOTOBOOTH:
					gameSettingsMap[game.id] = shared_ptr<PhotoboothSettings>(new PhotoboothSettings(model));
					break;

				case  game::id::FUNCES:
					gameSettingsMap[game.id]  = shared_ptr<FuncesSettings>(new FuncesSettings(model));	
					break;

				default:
					continue;
				}

				try	
				{				
					gameSettingsMap[game.id]->load();
				}
				catch(...)
				{
					throw ExcConfigFileParsing();
				}
			}
		}
		
		bool isGameID(int id)
		{
			vector<GamesInfo> games = model->getGames();

			for (auto game: games)
			{
				//if (!game.isOn || !game.isPurchased)
				//	continue;

				if(game.id == id)
					return true;
			}
		
			return false;
		}

		bool isGameCurrent(int id)
		{
			return currentGame == (game::id)id;
		}

		game::id getCurrentGame()
		{
			return currentGame;
		}

		void setCurrentGame(game::id id)
		{
			currentGame =  id;
		}

		void setNextGameId(game::id id)
		{
			nextGameId = id;
		}

		game::id getNextGameId()
		{
			return nextGameId;
		}

		GamesDataStruct getData()
		{
			return data;
		}

		void setData(GamesDataStruct _data)
		{
			data = _data;

			model->setGames(data.games);
			model->setDefaultGameID((game::id)data.defaultGameID);
			model->saveConfig();
		}

		bool isCurrentGameInSwitchOnGames()
		{
			return data.isIdInSwitchOnGames(currentGame);
		}

	private:

		game::id currentGame, nextGameId;	
		map<game::id, shared_ptr<ISettings>> gameSettingsMap;

		GamesDataStruct data;
	};
}