#pragma once

#include "ISettings.h"
#include "FuncesSettings.h"
#include "PhotoboothSettings.h"
#include "InstakubSettings.h"
#include "Types.h"
#include "ApplicationModel.h"
#include "ConfigSettings.h"
#include "GamesFactory.h"
#include "Photobooth.h"
#include "instakub/Instakub.h"
#include "Funces.h"
#include "Poza.h"
#include "PozaSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class GameSettings> GameSettingsRef;

		class GameSettings :public ISettings
		{
		public:
			class GamesDataStruct
			{
				std::vector<GamesInfo> games;
				int defaultGameID;

			public:
				int getDefaultGameID();
				int getGameID(int i);
				void setDefaultGameID(int value);

				std::vector<GamesInfo> getGames();
				int getCountSwitchOnGames();
				bool isIdInSwitchOnGames(const GameId& id);

				std::vector<GamesInfo> getNotPurchasedGames();
				std::vector<GamesInfo> getPurchasedGames();
				GamesInfo getPurchasedGameInfo(const GameId& id);

				friend GameSettings;
			};

			GameSettings(ApplicationModelRef model, ConfigSettingsRef configSettings);

			virtual void createMemento();
			virtual void writeConfig();

			ISettingsRef get(const GameId& id);
			bool has(const GameId& id);

			IResourceDictionary getActiveGameResources();
			IResourceDictionary getGameTexturesById(const GameId& id);
			IResourceDictionary getGameSettingsTexturesById(const GameId& id);	
			vector<IResourceBaseRef> getGameSettingsTextures();
			IResourceDictionary getActiveGameTextures();			

			bool settingsChanged();
			changeSetting::id getChangeID() const;
			
			void setAllTextures();
			void setTextures() override;
			void setTextures(const GameId& id);
			void load() override;
			void buildSettingData() override;
			void buildLocationData() override;

			bool isGameID(int id);
			bool isGameCurrent(int id);

			GameId getCurrentGame();
			void setCurrentGame(const GameId& id);

			void setNextGameId(const GameId& id);
			GameId getNextGameId();

			GamesDataStruct getData();
			void setData(GamesDataStruct data);

			bool isCurrentGameInSwitchOnGames();
			void setGameActive(const GameId& id, bool value);
			int getGameActiveCount();
			GameId getActiveGameID();

			std::string getGameDescribeURL(const GameId& id);
			std::string getGameStatisticURL(const GameId& id);

			GamesFactory<IGame>::base_ptr createGame(const GameId& id);
			void gamesfactoryReg();

		private:
			bool memento;
			GameId currentGame, nextGameId;
			std::map<GameId, ISettingsRef> gameSettingsMap;
			GamesDataStruct data, dataMemento;

			ConfigSettingsRef configSettings;
			GamesFactory<IGame>	gamesFactory;
		};
	}
}