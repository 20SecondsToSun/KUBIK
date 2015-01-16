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
			lang = "ru";
			
			JsonTree configJSON		= JsonTree(loadFile(getConfigPath()));
			userID					= configJSON.getChild("userID").getValue<string>();
			//actionName				= configJSON.getChild("actionName").getValue<string>();
			standID					= configJSON.getChild("standID").getValue<int>();
			netConnection			= configJSON.getChild("netConnection").getValue<bool>();
			defaultGameID			= (game::id)configJSON.getChild("defaultGameID").getValue<int>();	

			screenSaverConfigPath	= configJSON.getChild("screenSaverConfigPath").getValue<string>();

			menuConfigPath			= configJSON.getChild("menuConfigPath").getValue<string>();
			tuneUpConfigPath		= configJSON.getChild("tuneUpConfigPath").getValue<string>();				
			photoboothConfigPath	= configJSON.getChild("photoboothConfigPath").getValue<string>();
			funcesConfigPath		= configJSON.getChild("funcesConfigPath").getValue<string>();
			instagramConfigPath		= configJSON.getChild("instagramConfigPath").getValue<string>();
			kotopozaConfigPath		= configJSON.getChild("kotopozaConfigPath").getValue<string>();

			JsonTree gamesAvailable = JsonTree(configJSON.getChild("gamesAvailable"));
			JsonTree gamesPurchased = JsonTree(configJSON.getChild("gamesPurchased"));
			JsonTree gamesTurnOn	= JsonTree(configJSON.getChild("gamesTurnOn"));

			vector<int> purchasedGames, turnOnGames;

			for(auto it : gamesPurchased)
				purchasedGames.push_back(it.getChild("id").getValue<int>());

			for(auto it : gamesTurnOn)
				turnOnGames.push_back(it.getChild("id").getValue<int>());

			string iconUrl = configJSON.getChild("iconPath").getValue<string>();

			for(auto it : gamesAvailable)
			{
				GamesInfo game;
				game.id	  = (GameId)it.getChild("id").getValue<int>();
				game.isOn = findGameId(game.id, turnOnGames);
				game.isPurchased = findGameId(game.id, purchasedGames);
				game.name = it.getChild("name").getValue<string>();				
				
				game.setActiveIcon(loadImage(getFullPath(iconUrl + it.getChild("iconOn").getValue<string>())));
				game.setUnActiveIcon(loadImage(getFullPath(iconUrl + it.getChild("iconOff").getValue<string>())));
				games.push_back(game);	
			}			
		}

		void saveConfig()
		{
			fs::path basePath(getConfigPath());

			JsonTree doc;		
			doc.addChild(JsonTree("userID", userID));
			//doc.addChild(JsonTree("actionName", actionName));
			doc.addChild(JsonTree("standID", standID));
			doc.addChild(JsonTree("netConnection", netConnection));
			doc.addChild(JsonTree("defaultGameID", defaultGameID));
			doc.addChild(JsonTree("screenSaverConfigPath", screenSaverConfigPath));
			doc.addChild(JsonTree("menuConfigPath", menuConfigPath));	
			doc.addChild(JsonTree("tuneUpConfigPath", tuneUpConfigPath));	
			doc.addChild(JsonTree("photoboothConfigPath", photoboothConfigPath));	
			doc.addChild(JsonTree("funcesConfigPath", funcesConfigPath));	
			doc.addChild(JsonTree("instagramConfigPath", instagramConfigPath));	
			doc.addChild(JsonTree("kotopozaConfigPath", kotopozaConfigPath));	

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

		/*string getActionName()
		{
			return actionName;
		}*/

		bool findGameId(int id, vector<int> gamesTurnOn)
		{
			for (auto it:gamesTurnOn)
			{
				if(it == id)
					return true;
			}
			return false;
		}

	/*	string getNameById(game::id id)
		{			
			switch (id)
			{
			case game::id::PHOTOBOOTH:
				return "Photobooth";

			case  game::id::FUNCES:
				return "Funces";
			}	
			return "none";
		}*/


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

		string getFullPath(string path)
		{
			return  getAppPath().string() + path;
		}

		string getLang()
		{
			return lang;
		}

	private:
		string userID;
		int standID;
		bool netConnection;
		game::id defaultGameID;	

		string lang;
		
	//	string actionName;
		string menuConfigPath;
		string tuneUpConfigPath;
		string screenSaverConfigPath;
		string photoboothConfigPath;
		string funcesConfigPath;
		string instagramConfigPath;
		string kotopozaConfigPath;

		fs::path getConfigPath()
		{		
			return getAppPath() / "data/configs/app.txt";
		}

		vector<GamesInfo> games;
	};

	typedef shared_ptr<ApplicationModel> ApplicationModelRef;	
}