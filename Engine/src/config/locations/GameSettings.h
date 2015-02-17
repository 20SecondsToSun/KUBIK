#pragma once

#include "ISettings.h"
#include "FuncesSettings.h"
#include "PhotoboothSettings.h"
#include "Types.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik::config;

namespace kubik
{
	class GameSettings:public ISettings
	{
	public:

		class GamesDataStruct
		{
			vector<GamesInfo> games;
			int defaultGameID;	
			
		public:
			int getDefaultGameID()
			{
				return defaultGameID;	
			}

			int getGameID(int i)
			{
				return games[i].id;	
			}

			void setDefaultGameID(int value)
			{
				defaultGameID = value;
			}

			vector<GamesInfo> getGames()
			{
				return games;	
			}

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

			bool isIdInSwitchOnGames(GameId id)
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

			vector<GamesInfo> getNotPurchasedGames()
			{
				vector<GamesInfo> _gamesSelect;
				for (auto game: games)
				{
					if(!game.isPurchased)
						_gamesSelect.push_back(game);
				}
				return _gamesSelect;
			}

			vector<GamesInfo> getPurchasedGames()
			{
				vector<GamesInfo> _gamesSelect;
				for (auto game: games)
				{
					if(game.isPurchased)
						_gamesSelect.push_back(game);
				}
				return _gamesSelect;
			}

			GamesInfo getPurchasedGameInfo(GameId id)
			{
				for (auto game: games)
				{
					if(game.isPurchased && id == game.id)
						return game;
				}

				GamesInfo null;

				return null;
			}

			friend GameSettings;
		};

		GameSettings(ApplicationModelRef model):ISettings(model)
		{
			currentGame		   = model->getDefaultGameID();
			data.games		   = model->getGames();
			data.defaultGameID = model->getDefaultGameID();
		}

		ISettingsRef get(GameId id)
		{
			return gameSettingsMap[id];
		}		

		IResourceDictionary getActiveGameResources()
		{
			return getGameTexturesById(currentGame);
		}

		IResourceDictionary getGameTexturesById(GameId id)
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
				if (!game.isPurchased)
					continue;

				switch (game.id)
				{
				case  GameId::PHOTOBOOTH:
					gameSettingsMap[game.id] = PhotoboothSettingsRef(new PhotoboothSettings(model));
					break;

				case  GameId::FUNCES:
					gameSettingsMap[game.id]  = FuncesSettingsRef(new FuncesSettings(model));	
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

		void buildData()
		{
			vector<GamesInfo> games = model->getGames();
			for (auto game: games)
			{
				if (!game.isPurchased)
					continue;
				try	
				{	
					if(GameId::PHOTOBOOTH == game.id)
					gameSettingsMap[game.id]->buildData();
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
				if(game.id == id)
					return true;
			}
		
			return false;
		}

		bool isGameCurrent(int id)
		{
			return currentGame == (GameId)id;
		}

		game::id getCurrentGame()
		{
			return currentGame;
		}

		void setCurrentGame(GameId id)
		{
			currentGame =  id;
		}

		void setNextGameId(GameId id)
		{
			nextGameId = id;
		}

		GameId getNextGameId()
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
			model->setDefaultGameID((GameId)data.defaultGameID);
			model->saveUserData();
		}

		bool isCurrentGameInSwitchOnGames()
		{
			return data.isIdInSwitchOnGames(currentGame);
		}

	private:
		GameId currentGame, nextGameId;	
		map<GameId, ISettingsRef> gameSettingsMap;
		GamesDataStruct data;
	};

	typedef shared_ptr<GameSettings> GameSettingsRef;
}