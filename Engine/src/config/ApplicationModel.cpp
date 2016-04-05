#include "ApplicationModel.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik::config;
using namespace kubik;

void ApplicationModel::load()
{
	logger().log("PARSE CONFIG DATA");
	parseConfigPaths();
	logger().log("PARSE USER DATA");
	parseUserData();
	logger().log("PARSE DESIGN DATA");
	parseDesignData();
}

////////////////////////////////////////////////////////////////////////////
//
//					CONFIG PATH
//
////////////////////////////////////////////////////////////////////////////

void ApplicationModel::parseConfigPaths()
{
	auto path = getConfigPath();
	JsonTree configJSON   = JsonTree(loadFile(getConfigPath()));
	screenSaverConfigPath = configJSON.getChild("screenSaverConfigPath").getValue<string>();
	menuConfigPath		  = configJSON.getChild("menuConfigPath").getValue<string>();
	userDataPath		  = configJSON.getChild("userInfoPath").getValue<string>();
	labelsPath			  = configJSON.getChild("labelsPath").getValue<string>();
	designDataPath		  = configJSON.getChild("designDataPath").getValue<string>();
	socSettingsFilePath   = configJSON.getChild("socialConfigPath").getValue<string>();

	JsonTree pozaJSON = configJSON.getChild("pozaConfig");
	parseConfigPaths(pozaConfigObject, pozaJSON);
	configObjectMap[settings::id::POZA] = pozaConfigObject;

	JsonTree phtJSON = configJSON.getChild("photoboothConfig");
	parseConfigPaths(photoboothConfigObject, phtJSON);
	configObjectMap[settings::id::PHOTOBOOTH] = photoboothConfigObject;

	JsonTree instaJSON = configJSON.getChild("instakubConfig");
	parseConfigPaths(instakubConfigObject, instaJSON);
	configObjectMap[settings::id::INSTAKUB] = instakubConfigObject;

	JsonTree funcesJSON = configJSON.getChild("funcesConfig");
	parseConfigPaths(funcesConfigObject, funcesJSON);
	configObjectMap[settings::id::FUNCES] = funcesConfigObject;

	JsonTree mainJSON = configJSON.getChild("mainConfig");
	parseConfigPaths(mainConfigObject, mainJSON);
	configObjectMap[settings::id::MAINCONFIG] = mainConfigObject;

	preloadersConfigObject.setPathsConfigPath(getFullPath(configJSON.getChild("preloadersConfigPath").getValue<string>()));
	configObjectMap[settings::id::PRELOADERS] = preloadersConfigObject;
}

void ApplicationModel::parseConfigPaths(ConfigObject& configObject, const JsonTree& json)
{
	configObject.setPathsConfigPath(getFullPath(json.getChild("path").getValue<string>()));
	configObject.setParamsConfigPath(getFullPath(json.getChild("params").getValue<string>()));
	configObject.setLabelsConfigPath(getFullPath(json.getChild("labels").getValue<string>()));
	configObject.setConstsConfigPath(getFullPath(json.getChild("consts").getValue<string>()));
}

////////////////////////////////////////////////////////////////////////////
//
//					USER DATA
//
////////////////////////////////////////////////////////////////////////////

void ApplicationModel::parseUserData()
{
	JsonTree userInfoJSON   = JsonTree(loadFile(getUserDataPath()));

	_userDataPath		    = JsonTree(userInfoJSON.getChild("userData")).getValue<string>();
	JsonTree userDataJSON   = JsonTree(loadFile(getFullPath(_userDataPath)));

	lang					= userDataJSON.getChild("lang").getValue<string>();
	userID					= userDataJSON.getChild("userID").getValue<string>();
	standID					= userDataJSON.getChild("standID").getValue<int>();
	netConnection			= userDataJSON.getChild("netConnection").getValue<bool>();
	defaultGameID			= (GameId)userDataJSON.getChild("defaultGameID").getValue<int>();

	auto gamesAvailablePath = JsonTree(userInfoJSON.getChild("gamesAvailable")).getValue<string>();
	gamesPurchasedPath		= JsonTree(userInfoJSON.getChild("gamesPurchasedPath")).getValue<string>();
	gamesTurnOnPath			= JsonTree(userInfoJSON.getChild("gamesTurnOnPath")).getValue<string>();

	JsonTree gamesPurchased = JsonTree(loadFile(getFullPath(gamesPurchasedPath))).getChild("data");
	JsonTree gamesTurnOn	= JsonTree(loadFile(getFullPath(gamesTurnOnPath))).getChild("data");
	JsonTree gamesAvailable = JsonTree(loadFile(getFullPath(gamesAvailablePath))).getChild("data");

	vector<int> purchasedGames, turnOnGames;
	for (auto it : gamesPurchased)
	{
		purchasedGames.push_back(it.getChild("id").getValue<int>());
	}		

	for (auto it : gamesTurnOn)
	{
		turnOnGames.push_back(it.getChild("id").getValue<int>());
	}
	
	string iconUrl = userInfoJSON.getChild("iconPath").getValue<string>();

	for (auto it : gamesAvailable)
	{
		GamesInfo game;
		game.id = (GameId)it.getChild("id").getValue<int>();
		game.isOn = findGameId(game.id, turnOnGames);
		game.isPurchased = findGameId(game.id, purchasedGames);
		game.name = it.getChild("name").getValue<string>();
		auto iconPath = getFullPath(iconUrl + it.getChild("icon").getValue<string>());		
		game.setTexture(loadImage(iconPath));

		game.setActiveIcon(loadImage(getFullPath(iconUrl + it.getChild("iconOn").getValue<string>())));
		game.setUnActiveIcon(loadImage(getFullPath(iconUrl + it.getChild("iconOff").getValue<string>())));
		game.setMiniIcon(loadImage(getFullPath(iconUrl + it.getChild("miniIcon").getValue<string>())));
		games.push_back(game);
	}
}

////////////////////////////////////////////////////////////////////////////
//
//					DESIGN DATA
//
////////////////////////////////////////////////////////////////////////////

void ApplicationModel::parseDesignData()
{
	JsonTree designDataJSON = JsonTree(loadFile(getDesignDataPath()));
	JsonTree designs = designDataJSON.getChild("designs");

	for (auto it : designs)
	{
		OneDesignItem item;
		item.setID(it.getChild("id").getValue<int>());
		item.setIconPath(it.getChild("iconPath").getValue<string>());	

		item.setIconTexName("design" + item.getID());
		JsonTree text = it.getChild("textObj");

		item.setTextItem(text.getChild("text").getValue<string>(),
			text.getChild("font").getValue<string>(),
			text.getChild("size").getValue<int>(),
			text.getChild("color").getValue<string>());
		designData.push_back(item);
	}
	userDesignID = designDataJSON.getChild("userDesignID").getValue<int>();
}

////////////////////////////////////////////////////////////////////////////
//
//					SAVE
//
////////////////////////////////////////////////////////////////////////////

void ApplicationModel::writeGamesData(const std::vector<GamesInfo>& games)
{
	this->games = games;

	fs::path basePath(getFullPath(gamesTurnOnPath));

	JsonTree doc;
	JsonTree gamesTurnOn = JsonTree::makeArray("data");
	for (auto it : games)
	{
		if (it.isOn)
		{
			JsonTree id;
			id.addChild(JsonTree("id", it.id));
			gamesTurnOn.pushBack(id);
		}
	}

	doc.addChild(gamesTurnOn);
	doc.write(writeFile(basePath), JsonTree::WriteOptions());
}

void ApplicationModel::saveUserData()
{
	fs::path basePath(getFullPath(_userDataPath));

	JsonTree doc;
	doc.addChild(JsonTree("userID", userID));
	doc.addChild(JsonTree("lang", lang));
	doc.addChild(JsonTree("standID", standID));
	doc.addChild(JsonTree("netConnection", netConnection));
	doc.addChild(JsonTree("defaultGameID", defaultGameID));
	doc.write(writeFile(basePath), JsonTree::WriteOptions());
}

////////////////////////////////////////////////////////////////////////////
//
//				ACCESSORS
//
////////////////////////////////////////////////////////////////////////////

bool ApplicationModel::findGameId(int id, const vector<int>& gamesTurnOn)
{
	for (auto it : gamesTurnOn)
	{
		if (it == id)
		{
			return true;
		}			
	}	

	return false;
}

vector<GamesInfo> ApplicationModel::getGames()
{
	return games;
}

void ApplicationModel::setGames(const vector<GamesInfo>& value)
{
	games = value;
}

void ApplicationModel::setDefaultGameID(GameId value)
{
	defaultGameID = value;
}

GameId ApplicationModel::getDefaultGameID() const
{
	return defaultGameID;
}

bool ApplicationModel::onlyOneGameOn()
{
	int counter = 0;

	for (auto it : games)
	{
		if (it.isOn && ++counter > 1)
		{
			return false;
		}			
	}	

	return true;
}

GameId ApplicationModel::onlyOneGameOnID()
{
	int counter = 0;

	for (auto it : games)
	{
		if (it.isOn)
		{
			return it.id;
		}			
	}		

	return GameId::INSTAKUB;//TODO
}

////////////////////////////////////////////////////////////////////////////
//
//				SET
//
////////////////////////////////////////////////////////////////////////////

void ApplicationModel::setMenuConfigPath(const string& path)
{
	menuConfigPath = path;
}

void ApplicationModel::setScreenSaverPath(const string& path)
{
	screenSaverConfigPath = path;
}

////////////////////////////////////////////////////////////////////////////
//
//				GET
//
////////////////////////////////////////////////////////////////////////////

const ConfigObject& ApplicationModel::getConfigObject(const settings::id& id)
{
	return configObjectMap[id];	
}

string ApplicationModel::getLabelsPath()
{
	return getFullPath(labelsPath);
}

string ApplicationModel::getMenuConfigPath()
{
	return getFullPath(menuConfigPath);
}

string ApplicationModel::getScreenSaverConfigPath()
{
	return getFullPath(screenSaverConfigPath);
}

string ApplicationModel::getUserDataPath()
{
	return getFullPath(userDataPath);
}

string ApplicationModel::getDesignDataPath()
{
	return getFullPath(designDataPath);
}

string ApplicationModel::getFullPath(const string& path) const
{
	return Paths::getBasePath().string() + path;
}

string ApplicationModel::getLang() const
{
	return lang;
}

DesignData ApplicationModel::getDesignData() const
{
	return designData;
}

int ApplicationModel::getUserDesignID() const
{
	return userDesignID;
}

string ApplicationModel::getSocialSettingsFilePath() const
{
	return socSettingsFilePath;
}

fs::path ApplicationModel::getConfigPath()
{
	return Paths::getAppConfigFile();
}
