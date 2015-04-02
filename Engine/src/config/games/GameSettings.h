#pragma once

#include "ISettings.h"
#include "FuncesSettings.h"
#include "PhotoboothSettings.h"
#include "InstakubSettings.h"
#include "Types.h"
#include "ApplicationModel.h"
#include "ConfigSettings.h"

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
			IResourceDictionary getActiveGameResources();
			IResourceDictionary getGameTexturesById(const GameId& id);
			IResourceDictionary getActiveGameTextures();			

			bool settingsChanged();
			changeSetting::id getChangeID() const;

			void setTextures() override;
			void load() override;
			void buildData();

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

		private:
			bool memento;
			GameId currentGame, nextGameId;
			std::map<GameId, ISettingsRef> gameSettingsMap;
			GamesDataStruct data, dataMemento;

			ConfigSettingsRef configSettings;
		};
	}
}