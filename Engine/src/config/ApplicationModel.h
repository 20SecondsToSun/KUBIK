#pragma once

#include "KubikException.h"
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
			parseConfigPaths();				
			parseUserData();		
		}

		void parseConfigPaths()
		{
			JsonTree configJSON		= JsonTree(loadFile(getConfigPath()));
			screenSaverConfigPath	= configJSON.getChild("screenSaverConfigPath").getValue<string>();
			menuConfigPath			= configJSON.getChild("menuConfigPath").getValue<string>();
			tuneUpConfigPath		= configJSON.getChild("tuneUpConfigPath").getValue<string>();				
			photoboothConfigPath	= configJSON.getChild("photoboothConfigPath").getValue<string>();
			funcesConfigPath		= configJSON.getChild("funcesConfigPath").getValue<string>();
			instagramConfigPath		= configJSON.getChild("instagramConfigPath").getValue<string>();
			kotopozaConfigPath		= configJSON.getChild("kotopozaConfigPath").getValue<string>();
			userDataPath			= configJSON.getChild("userInfoPath").getValue<string>();

		}

		void parseUserData()
		{		
			JsonTree userInfoJSON	= JsonTree(loadFile(getUserDataPath()));
			lang					= userInfoJSON.getChild("lang").getValue<string>();	
			userID					= userInfoJSON.getChild("userID").getValue<string>();	
			standID					= userInfoJSON.getChild("standID").getValue<int>();
			netConnection			= userInfoJSON.getChild("netConnection").getValue<bool>();
			defaultGameID			= (GameId)userInfoJSON.getChild("defaultGameID").getValue<int>();			
				JsonTree gamesAvailable = JsonTree(userInfoJSON.getChild("gamesAvailable"));
			JsonTree gamesPurchased = JsonTree(userInfoJSON.getChild("gamesPurchased"));
			JsonTree gamesTurnOn	= JsonTree(userInfoJSON.getChild("gamesTurnOn"));

			vector<int> purchasedGames, turnOnGames;		
			for(auto it : gamesPurchased)
				purchasedGames.push_back(it.getChild("id").getValue<int>());
		
			for(auto it : gamesTurnOn)
				turnOnGames.push_back(it.getChild("id").getValue<int>());
		
			string iconUrl = userInfoJSON.getChild("iconPath").getValue<string>();			
		
			for(auto it : gamesAvailable)
			{
				GamesInfo game;
				game.id	  = (GameId)it.getChild("id").getValue<int>();
				game.isOn = findGameId(game.id, turnOnGames);
				game.isPurchased = findGameId(game.id, purchasedGames);
				game.name = it.getChild("name").getValue<string>();	
				
				game.setActiveIcon(loadImage(getFullPath(iconUrl + it.getChild("iconOn").getValue<string>())));
				game.setUnActiveIcon(loadImage(getFullPath(iconUrl + it.getChild("iconOff").getValue<string>())));
				game.setMiniIcon(loadImage(getFullPath(iconUrl + it.getChild("miniIcon").getValue<string>())));
				games.push_back(game);	
				
			}	
		}

		void saveUserData()
		{
			fs::path basePath(getConfigPath());

			JsonTree doc;		
			doc.addChild(JsonTree("userID", userID));
			doc.addChild(JsonTree("lang", lang));
			doc.addChild(JsonTree("standID", standID));
			doc.addChild(JsonTree("netConnection", netConnection));
			doc.addChild(JsonTree("defaultGameID", defaultGameID));
			
			JsonTree gamesJ			= JsonTree::makeArray("gamesAvailable");
			JsonTree gamesTurnOnJ   = JsonTree::makeArray("gamesTurnOn");
			JsonTree gamesPurchased = JsonTree::makeArray("gamesPurchased");

			for (auto it: games)
			{
				JsonTree id;
				id.addChild(JsonTree("id", it.id));
				gamesJ.pushBack( id);

				if (it.isOn)
				{
					JsonTree _id;
					_id.addChild(JsonTree("id", it.id));
					gamesTurnOnJ.pushBack( _id);
				}
				
				if(it.isPurchased)
				{
					JsonTree _id_;
					_id_.addChild(JsonTree("id", it.id));
					gamesPurchased.pushBack( _id_);
				}
			}

			doc.addChild(gamesJ);
			doc.addChild(gamesTurnOnJ);
			doc.addChild(gamesPurchased);
			doc.write(writeFile(basePath), JsonTree::WriteOptions());
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//				ACCESSORS
		//
		////////////////////////////////////////////////////////////////////////////

		bool findGameId(int id, vector<int> gamesTurnOn)
		{
			for (auto it:gamesTurnOn)			
				if(it == id)
					return true;
			
			return false;
		}

		vector<GamesInfo> getGames()
		{
			return games;
		}

		void setGames(vector<GamesInfo> _games)
		{
			games = _games;
		}
		
		void setDefaultGameID(game::id _value)
		{
			defaultGameID = _value;
		}

		game::id getDefaultGameID()
		{
			return defaultGameID;
		}

		bool onlyOneGameOn()
		{
			int counter = 0;
			for (auto it: games)
			{
				if(it.isOn)
					counter++;

				if(counter > 1)
					return false;
			}
			return true;
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//				SET PATH
		//
		////////////////////////////////////////////////////////////////////////////

		void setMenuConfigPath(string path)
		{
			menuConfigPath = path;
		}

		void setTuneUpConfigPath(string path)
		{
			tuneUpConfigPath = path;
		}	

		void setScreenSaverPath(string path)
		{
			screenSaverConfigPath =  path;
		}

		string getMenuConfigPath()
		{
			return getFullPath(menuConfigPath);
		}

		string getTuneUpConfigPath()
		{
			return getFullPath(tuneUpConfigPath);
		}

		string getScreenSaverConfigPath()
		{
			return getFullPath(screenSaverConfigPath);
		}

		string getPhotoboothConfigPath()
		{
			return getFullPath(photoboothConfigPath);
		}

		string getFuncesConfigPath()
		{
			return getFullPath(funcesConfigPath);
		}

		string getInstagramConfigPath()
		{
			return getFullPath(instagramConfigPath);
		}

		string getKotopozaConfigPath()
		{
			return getFullPath(kotopozaConfigPath);
		}
		string getUserDataPath()
		{
			return getFullPath(userDataPath);
		}

		string getFullPath(string path)
		{
			return  getAppPath().string() + path;
		}

		string getLang()
		{
			return lang;
		}

	private:		
		int	   standID;
		bool   netConnection;
		string userID;
		GameId defaultGameID;	
		string lang;		
		string menuConfigPath;
		string tuneUpConfigPath;
		string screenSaverConfigPath;
		string photoboothConfigPath;
		string funcesConfigPath;
		string instagramConfigPath;
		string kotopozaConfigPath;
		string userDataPath;		

		fs::path getConfigPath()
		{		
			return getAppPath() / "data/configs/app.txt";
		}

		vector<GamesInfo> games;
	};

	typedef shared_ptr<ApplicationModel> ApplicationModelRef;	
}