#pragma once
#include "cinder/app/AppNative.h"
#include <boost/algorithm/string.hpp>
#include "KubikException.h"
#include "ApplicationModel.h"
#include "cinder/Json.h"
#include "Types.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class ApplicationModel 
	{

	public:	

		void load()
		{
			JsonTree configJSON		= JsonTree(loadFile(getConfigPath()));
			userID					= configJSON.getChild("userID").getValue<string>();
			standID					= configJSON.getChild("standID").getValue<int>();
			netConnection			= configJSON.getChild("netConnection").getValue<bool>();
			defaultGameID			= configJSON.getChild("defaultGameID").getValue<int>();	

			screenSaverPath			= getAppPath().string() +  configJSON.getChild("screenSaverPath").getValue<string>();

			menuConfigPath			= getAppPath().string() +  configJSON.getChild("menuConfigPath").getValue<string>();
			tuneUpConfigPath		= getAppPath().string() +  configJSON.getChild("tuneUpConfigPath").getValue<string>();				
			photoboothConfigPath	= getAppPath().string() +  configJSON.getChild("photoboothConfigPath").getValue<string>();
			funcesConfigPath		= getAppPath().string() +  configJSON.getChild("funcesConfigPath").getValue<string>();
			instagramConfigPath		= getAppPath().string() +  configJSON.getChild("instagramConfigPath").getValue<string>();
			kotopozaConfigPath		= getAppPath().string() +  configJSON.getChild("kotopozaConfigPath").getValue<string>();

			JsonTree gamesAvailable = JsonTree(configJSON.getChild( "gamesAvailable"));
			JsonTree gamesPurchased = JsonTree(configJSON.getChild( "gamesPurchased"));
			JsonTree gamesTurnOn = JsonTree(configJSON.getChild( "gamesTurnOn"));

			vector<int> purchasedGames, turnOnGames;

			for(auto it : gamesPurchased)
				purchasedGames.push_back(it.getValue<int>());

			for(auto it : gamesTurnOn)
				turnOnGames.push_back(it.getValue<int>());
			
			for(auto it : gamesAvailable)
			{
				GamesInfo game;
				game.id	  = it.getValue<int>();
				game.isOn = findGameId(game.id, turnOnGames);
				game.isPurchased = findGameId(game.id, purchasedGames);
				game.name = getNameById(game.id);
				games.push_back(game);
			}
		}

		bool findGameId(int id, vector<int> gamesTurnOn)
		{
			for (auto it:gamesTurnOn)
			{
				if(it == id)
					return true;
			}
			return false;
		}

		string getNameById(int id)
		{			
			switch (id)
			{
			case gameId::PHOTOBOOTH:
				return "Photobooth";

			case gameId::FUNCES:
				return "Funces";
			}	
			return "none";
		}

		vector<GamesInfo> getGames()
		{
			return games;
		}

		void setUserID(string _value)
		{
			userID = _value;
		}

		void setStandID(int _value)
		{
			standID = _value;
		}

		void setNetConnection(bool _value)
		{
			netConnection = _value;
		}

		void setGameIDsAvailable(vector<int> _value)
		{
			gameIDsAvailable = _value;
		}

		void setGameIDsPurchased(vector<int> _value)
		{
			gameIDsPurchased = _value;
		}

		void setGameIDsTurnOn(vector<int> _value)
		{
			gameIDsTurnOn = _value;
		}

		void setDefaultGameID(int _value)
		{
			defaultGameID = _value;
		}

		int getDefaultGameID()
		{
			return defaultGameID;
		}	

		vector<int> getGameIDsTurnOn()
		{
			return gameIDsTurnOn;
		}

		vector<int> getGameIDsAvailable()
		{
			return gameIDsAvailable;
		}

		vector<int> getGamesPurchased()
		{
			return gameIDsPurchased;
		}

		bool checkIfGameIdPurchased(int id)
		{
			for(auto it = gameIDsPurchased.begin(); it != gameIDsPurchased.end(); ++it)
			{
				if (*it == id)
					return true;
			}

			return false;
		}

		bool onlyOneGameOn()
		{
			return gameIDsTurnOn.size() == 1;
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//				SET PATH
		//
		////////////////////////////////////////////////////////////////////////////

		void setMenuConfigPath(string path)
		{
			menuConfigPath = getAppPath().string() + path;
		}

		void setTuneUpConfigPath(string path)
		{
			tuneUpConfigPath = getAppPath().string() + path;
		}	

		void setScreenSaverPath(string path)
		{
			screenSaverPath = getAppPath().string() + path;
		}

		string getMenuConfigPath()
		{
			return menuConfigPath;
		}

		string getTuneUpConfigPath()
		{
			return tuneUpConfigPath;
		}

		string getScreenSaverPath()
		{
			return screenSaverPath;
		}

		string getPhotoboothConfigPath()
		{
			return photoboothConfigPath;
		}

		string getFuncesConfigPath()
		{
			return funcesConfigPath;
		}

		string getInstagramConfigPath()
		{
			return instagramConfigPath;
		}

		string getKotopozaConfigPath()
		{
			return kotopozaConfigPath;
		}

	private:

		string userID;
		int standID;
		int defaultGameID;
		bool netConnection;	

		string menuConfigPath;
		string tuneUpConfigPath;
		string screenSaverPath;
		string photoboothConfigPath;
		string funcesConfigPath;
		string instagramConfigPath;
		string kotopozaConfigPath;

		vector<int> gameIDsAvailable;
		vector<int> gameIDsPurchased;
		vector<int> gameIDsTurnOn;	

		fs::path getConfigPath()
		{		
			return getAppPath() / "data/configs/app.txt";
		}

		vector<GamesInfo> games;
	};
}