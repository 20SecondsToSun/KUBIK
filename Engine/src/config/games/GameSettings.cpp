#include "GameSettings.h"
using namespace ci;
using namespace ci::app;
using namespace kubik::config;
using namespace kubik;
using namespace kubik::games::photobooth;
using namespace kubik::games::instakub;
using namespace kubik::games::funces;

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
		if (game.isOn)
			count++;	

	return count;
}

bool GameSettings::GamesDataStruct::isIdInSwitchOnGames(const GameId& id)
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

GamesInfo GameSettings::GamesDataStruct::getPurchasedGameInfo(const GameId& id)
{
	for (auto game : games)
	{
		if (game.isPurchased && id == game.id)
			return game;
	}

	GamesInfo null;

	return null;
}

GameSettings::GameSettings(ApplicationModelRef model, ConfigSettingsRef configSettings)
	: ISettings(model), memento(false), configSettings(configSettings)
{
	currentGame = model->getDefaultGameID();
	data.games  = model->getGames();
	data.defaultGameID = model->getDefaultGameID();
}

ISettingsRef GameSettings::get(const GameId& id)
{
	return gameSettingsMap[id];
}

IResourceDictionary GameSettings::getActiveGameResources()
{
	return getGameTexturesById(currentGame);
}

IResourceDictionary GameSettings::getGameTexturesById(const GameId& id)
{
	return  gameSettingsMap[id]->getResources();
}

IResourceDictionary GameSettings::getGameSettingsTexturesById(const GameId& id)
{
	return  gameSettingsMap[id]->getSettingsResources();
}

vector<IResourceBaseRef> GameSettings::getGameSettingsTextures()
{
	std::vector<GamesInfo> games = model->getGames();

	vector<IResourceBaseRef> resources;

	for (auto game : games)
	{
		if (!game.isPurchased) continue;

		try
		{
			auto oneGameResources = gameSettingsMap[game.id]->getSettingsResources();
			for (auto it = oneGameResources.begin(); it != oneGameResources.end(); it++)
				resources.push_back((*it).second);
		}
		catch (...)
		{
			throw ExcConfigFileParsing();
		}
	}

	return resources;
}

IResourceDictionary GameSettings::getActiveGameTextures()
{
	auto id = model->getDefaultGameID();// getActiveGameID();
	//console() << "active resources:::::::::::::  " << id << endl;
	return  gameSettingsMap[id]->getResources();
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
	//gameSettingsMap[currentGame]->lo
	gameSettingsMap[currentGame]->setTextures();
}

void GameSettings::load()
{
	std::vector<GamesInfo> games = model->getGames();
	bool error = false;

	for (auto game : games)
	{	
		if (!game.isPurchased)
			continue;

		switch (game.id)
		{
		case  GameId::PHOTOBOOTH:
			gameSettingsMap[game.id] = PhotoboothSettingsRef(new PhotoboothSettings(model, configSettings));
			break;

		case  GameId::FUNCES:
			gameSettingsMap[game.id] = FuncesSettingsRef(new FuncesSettings(model, configSettings));
			break;

		case  GameId::INSTAKUB:
			gameSettingsMap[game.id] = InstakubSettingsRef(new InstakubSettings(model, configSettings));
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
			error = true;
			//throw ExcConfigFileParsing();
		}
	}

	if (error)
		throw ExcConfigFileParsing();
}

void GameSettings::buildLocationData()
{
	auto id = model->getDefaultGameID();
	try
	{
		gameSettingsMap[id]->buildLocationData();
	}
	catch (...)
	{
		throw ExcConfigFileParsing();
	}
}

void GameSettings::buildSettingData()
{
	std::vector<GamesInfo> games = model->getGames();

	for (auto game : games)
	{
		if (!game.isPurchased) continue;

		try
		{
			gameSettingsMap[game.id]->buildSettingData();
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

GamesFactory<IGame>::base_ptr GameSettings::createGame(const GameId& id)
{
	return gamesFactory.create(id);
}

void GameSettings::gamesfactoryReg()
{
	gamesFactory.reg<Instakub>(GameId::INSTAKUB, gameSettingsMap[GameId::INSTAKUB]);
	gamesFactory.reg<Photobooth>(GameId::PHOTOBOOTH, gameSettingsMap[GameId::PHOTOBOOTH]);
}

GameId GameSettings::getCurrentGame()
{
	return currentGame;
}

void GameSettings::setCurrentGame(const GameId& id)
{
	currentGame = id;
}

void GameSettings::setNextGameId(const GameId& id)
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

void GameSettings::setGameActive(const GameId& id, bool value)
{
	for (auto game = data.games.begin(); game != data.games.end(); ++game)
	{
		if (game->isPurchased && game->id == id)
			game->isOn = value;
	}
}

int GameSettings::getGameActiveCount()
{
	int count = 0;
	for (auto game = data.games.begin(); game != data.games.end(); ++game)	
		if (game->isOn)
			count++;

	return count;	
}

GameId GameSettings::getActiveGameID()
{
	for (auto game = data.games.begin(); game != data.games.end(); ++game)
	{
		if (game->isOn)
			return game->getGameId();
	}

	return GameId::UNDEFINED;
}

string GameSettings::getGameDescribeURL(const GameId& id)
{
	return "http://google.ru";
}

string GameSettings::getGameStatisticURL(const GameId& id)
{
	return "http://yandex.ru";
}