#pragma once
#include "cinder/app/AppNative.h"
#include <boost/algorithm/string.hpp>
#include "cinder/Json.h"

#include "ApplicationModel.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class Config
{
public:

	static Config& getInstance() { static Config conf; return conf; };

	void load(ApplicationModel *model)
	{
		try	
		{
			JsonTree configJSON = JsonTree(loadFile(getConfigPath()));
			model->setUserID(configJSON.getChild("userID").getValue<string>());
			model->setStandID(configJSON.getChild("standID").getValue<int>());
			model->setNetConnection(configJSON.getChild("netConnection").getValue<bool>());

			int gameID = configJSON.getChild("defaultGameID").getValue<int>();
			model->setDefaultGameID(gameID);
			model->setCurrentGame(gameID);

			vector<int> temp;
			
			JsonTree gamesAvailable = JsonTree(configJSON.getChild( "gamesAvailable"));
			for(auto it = gamesAvailable.begin(); it != gamesAvailable.end(); ++it)
				temp.push_back(it->getValue<int>());

			model->setGameIDsAvailable(temp);
			temp.clear();

			JsonTree gamesPurchased = JsonTree(configJSON.getChild( "gamesPurchased"));
			for(auto it = gamesPurchased.begin(); it != gamesPurchased.end(); ++it)
				temp.push_back(it->getValue<int>());

			model->setGameIDsPurchased(temp);
			temp.clear();

			JsonTree gamesTurnOn = JsonTree(configJSON.getChild( "gamesTurnOn"));
			for(auto it = gamesTurnOn.begin(); it != gamesTurnOn.end(); ++it)
				temp.push_back(it->getValue<int>());

			model->setGameIDsTurnOn(temp);
			temp.clear();
		}
		catch(...)
		{
			errorHandler();
			return;
		}

		if (completeHandler)
			completeHandler();
	}

	void addCompleteListener(const std::function<void(void)>& handler)
	{
		completeHandler = handler;
	}

	void addErrorListener(const std::function<void(void)>& handler)
	{
		errorHandler = handler;
	}

private:
	std::function<void(void)> completeHandler;
	std::function<void(void)> errorHandler;

	fs::path getConfigPath()
	{
		fs::path basePath = getAppPath();
		return basePath / "data/config.txt";
	}
};

inline Config&	config() { return Config::getInstance(); };