#pragma once

#include "ISettings.h"
#include "FuncesSettings.h"
#include "PhotoboothSettings.h"
#include "InstakubSettings.h"
#include "Types.h"

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
			std::vector<GamesInfo> games;
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

			std::vector<GamesInfo> getGames()
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

			std::vector<GamesInfo> getNotPurchasedGames()
			{
				std::vector<GamesInfo> _gamesSelect;
				for (auto game: games)
				{
					if(!game.isPurchased)
						_gamesSelect.push_back(game);
				}
				return _gamesSelect;
			}

			std::vector<GamesInfo> getPurchasedGames()
			{
				std::vector<GamesInfo> _gamesSelect;
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

		virtual void createMemento(){};
		virtual void writeConfig(){};
		bool settingsChanged(){return false;};	
		changeSetting::id getChangeID(){ return changeSetting::id::GAMES;};	

		void setTextures() override
		{
			gameSettingsMap[currentGame]->setTextures();	
		}

		void load() override
		{
			std::vector<GamesInfo> games = model->getGames();

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

				case  GameId::INSTAKUB:
					gameSettingsMap[game.id]  = InstakubSettingsRef(new InstakubSettings(model));	
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
			std::vector<GamesInfo> games = model->getGames();
			for (auto game: games)
			{
				if (!game.isPurchased)
					continue;
				try	
				{	
					if(GameId::PHOTOBOOTH == game.id)
						gameSettingsMap[game.id]->buildData();
					if(GameId::INSTAKUB == game.id)
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
			std::vector<GamesInfo> games = model->getGames();

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

		GameId getCurrentGame()
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

		void setGameActive(GameId id, bool value)
		{
			for (auto game = data.games.begin(); game != data.games.end(); ++game)				
			{
				if(game->isPurchased && game->id == id)
						game->isOn = value;
			}				
		}

	private:
		GameId currentGame, nextGameId;	
		std::map<GameId, ISettingsRef> gameSettingsMap;
		GamesDataStruct data;
	};

	typedef shared_ptr<GameSettings> GameSettingsRef;
}