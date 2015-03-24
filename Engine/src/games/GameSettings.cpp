#include "GameSettings.h"
using namespace ci;
using namespace ci::app;
using namespace kubik::config;
using namespace kubik;

int GameSettings::GamesDataStruct::getDefaultGameID()
{
	return defaultGameID;
}

int GameSettings::GamesDataStruct::getGameID(int i)
{
	return games[i].id;
}

void GameSettings::GamesDataStruct::setDefaultGameID(int value)
{
	defaultGameID = value;
}

std::vector<GamesInfo> GameSettings::GamesDataStruct::getGames()
{
	return games;
}

int GameSettings::GamesDataStruct::getCountSwitchOnGames()
{
	int count = 0;

	for (auto game : games)
	{
		if (game.isOn)
			count++;
	}

	return count;
}

bool GameSettings::GamesDataStruct::isIdInSwitchOnGames(GameId id)
{
	for (auto game : games)
	{
		if (game.isOn)
		{
			if (game.id == id)
				return true;
		}
	}
	return false;
}

std::vector<GamesInfo> GameSettings::GamesDataStruct::getNotPurchasedGames()
{
	std::vector<GamesInfo> _gamesSelect;
	for (auto game : games)
	{
		if (!game.isPurchased)
			_gamesSelect.push_back(game);
	}
	return _gamesSelect;
}

std::vector<GamesInfo> GameSettings::GamesDataStruct::getPurchasedGames()
{
	std::vector<GamesInfo> _gamesSelect;
	for (auto game : games)
	{
		if (game.isPurchased)
			_gamesSelect.push_back(game);
	}
	return _gamesSelect;
}

GamesInfo GameSettings::GamesDataStruct::getPurchasedGameInfo(GameId id)
{
	for (auto game : games)
	{
		if (game.isPurchased && id == game.id)
			return game;
	}

	GamesInfo null;

	return null;
}

GameSettings::GameSettings(ApplicationModelRef model) :ISettings(model), memento(false)
{
	currentGame = model->getDefaultGameID();
	data.games = model->getGames();
	data.defaultGameID = model->getDefaultGameID();
}

ISettingsRef GameSettings::get(GameId id)
{
	return gameSettingsMap[id];
}

IResourceDictionary GameSettings::getActiveGameResources()
{
	return getGameTexturesById(currentGame);
}

IResourceDictionary GameSettings::getGameTexturesById(GameId id)
{
	IResourceDictionary rd = gameSettingsMap[id]->getResources();
	return rd;
}

void GameSettings::createMemento()
{
	memento = true;
	dataMemento = data;
};

void GameSettings::writeConfig()
{
	if (memento)
	{
		model->writeGamesData(data.games);
		memento = false;
	}
};

bool GameSettings::settingsChanged()
{
	for (size_t i = 0; i < data.games.size(); i++)
	{
		auto game = data.games[i];
		auto gameMemento = dataMemento.games[i];

		if (game.id == gameMemento.id)
			if (game.isOn != gameMemento.isOn)
				return true;
	}

	return false;
};

changeSetting::id GameSettings::getChangeID() const
{
	return changeSetting::id::GAMES;
};

void GameSettings::setTextures()
{
	gameSettingsMap[currentGame]->setTextures();
}

void GameSettings::load()
{
	std::vector<GamesInfo> games = model->getGames();

	for (auto game : games)
	{
		if (!game.isPurchased)
			continue;

		switch (game.id)
		{
		case  GameId::PHOTOBOOTH:
			gameSettingsMap[game.id] = PhotoboothSettingsRef(new PhotoboothSettings(model));
			break;

		case  GameId::FUNCES:
			gameSettingsMap[game.id] = FuncesSettingsRef(new FuncesSettings(model));
			break;

		case  GameId::INSTAKUB:
			gameSettingsMap[game.id] = InstakubSettingsRef(new InstakubSettings(model));
			break;

		default:
			continue;
		}

		try
		{
			gameSettingsMap[game.id]->load();
		}
		catch (...)
		{
			throw ExcConfigFileParsing();
		}
	}
}

void GameSettings::buildData()
{
	std::vector<GamesInfo> games = model->getGames();
	for (auto game : games)
	{
		if (!game.isPurchased)
			continue;
		try
		{
			if (GameId::PHOTOBOOTH == game.id)
				gameSettingsMap[game.id]->buildData();
			if (GameId::INSTAKUB == game.id)
				gameSettingsMap[game.id]->buildData();
		}
		catch (...)
		{
			throw ExcConfigFileParsing();
		}
	}
}

bool GameSettings::isGameID(int id)
{
	std::vector<GamesInfo> games = model->getGames();

	for (auto game : games)
	{
		if (game.id == id)
			return true;
	}

	return false;
}

bool GameSettings::isGameCurrent(int id)
{
	return currentGame == (GameId)id;
}

GameId GameSettings::getCurrentGame()
{
	console() << "getCurrentGame::::::::::::------------------------  " << currentGame << endl;
	return currentGame;
}

void GameSettings::setCurrentGame(GameId id)
{
	currentGame = id;
}

void GameSettings::setNextGameId(GameId id)
{
	nextGameId = id;
}

GameId GameSettings::getNextGameId()
{
	return nextGameId;
}

GameSettings::GamesDataStruct GameSettings::getData()
{
	return data;
}

void GameSettings::setData(GameSettings::GamesDataStruct data)
{
	this->data = data;

	model->setGames(data.games);
	model->setDefaultGameID((GameId)data.defaultGameID);
	model->saveUserData();
}

bool GameSettings::isCurrentGameInSwitchOnGames()
{
	return data.isIdInSwitchOnGames(currentGame);
}

void GameSettings::setGameActive(GameId id, bool value)
{
	for (auto game = data.games.begin(); game != data.games.end(); ++game)
	{
		if (game->isPurchased && game->id == id)
			game->isOn = value;
	}
}