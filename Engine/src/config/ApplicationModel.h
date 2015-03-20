#pragma once

#include "KubikException.h"
#include "Types.h"
#include "DesignData.h"
#include "ConfigObject.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace kubik::config;

namespace kubik
{
	class ApplicationModel 
	{
	public:
		void load()
		{	
			logger().log("parse config data");
			parseConfigPaths();	
			logger().log("parse user data");
			parseUserData();
			logger().log("parse design data");
			parseDesignData();
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//					CONFIG PATH
		//
		////////////////////////////////////////////////////////////////////////////

		void parseConfigPaths()
		{
			JsonTree configJSON		= JsonTree(loadFile(getConfigPath()));
			screenSaverConfigPath	= configJSON.getChild("screenSaverConfigPath").getValue<string>();
			menuConfigPath			= configJSON.getChild("menuConfigPath").getValue<string>();			
			funcesConfigPath		= configJSON.getChild("funcesConfigPath").getValue<string>();
			instagramConfigPath		= configJSON.getChild("instagramConfigPath").getValue<string>();
			kotopozaConfigPath		= configJSON.getChild("kotopozaConfigPath").getValue<string>();
			userDataPath			= configJSON.getChild("userInfoPath").getValue<string>();
			labelsPath				= configJSON.getChild("labelsPath").getValue<string>();
			designDataPath			= configJSON.getChild("designDataPath").getValue<string>();
			
			JsonTree phtJSON = configJSON.getChild("photoboothConfig");
			photoboothConfigObject.setPathsConfigPath(getFullPath(phtJSON.getChild("path").getValue<string>()));
			photoboothConfigObject.setParamsConfigPath(getFullPath(phtJSON.getChild("params").getValue<string>()));
			photoboothConfigObject.setLabelsConfigPath(getFullPath(phtJSON.getChild("labels").getValue<string>()));
			photoboothConfigObject.setConstsConfigPath(getFullPath(phtJSON.getChild("consts").getValue<string>()));

			JsonTree instaJSON = configJSON.getChild("instakubConfig");
			instakubConfigObject.setPathsConfigPath(getFullPath(instaJSON.getChild("path").getValue<string>()));
			instakubConfigObject.setParamsConfigPath(getFullPath(instaJSON.getChild("params").getValue<string>()));
			instakubConfigObject.setLabelsConfigPath(getFullPath(instaJSON.getChild("labels").getValue<string>()));

			JsonTree mainJSON = configJSON.getChild("mainConfig");
			mainConfigObject.setPathsConfigPath(getFullPath(mainJSON.getChild("path").getValue<string>()));
			mainConfigObject.setParamsConfigPath(getFullPath(mainJSON.getChild("params").getValue<string>()));
			mainConfigObject.setLabelsConfigPath(getFullPath(mainJSON.getChild("labels").getValue<string>()));
			mainConfigObject.setConstsConfigPath(getFullPath(mainJSON.getChild("consts").getValue<string>()));
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//					USER DATA
		//
		////////////////////////////////////////////////////////////////////////////

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
				
				game.setActiveIcon(loadImage(getFullPath(iconUrl   + it.getChild("iconOn").getValue<string>())));
				game.setUnActiveIcon(loadImage(getFullPath(iconUrl + it.getChild("iconOff").getValue<string>())));
				game.setMiniIcon(loadImage(getFullPath(iconUrl     + it.getChild("miniIcon").getValue<string>())));
				game.setTexture(loadImage(getFullPath(iconUrl	   + it.getChild("icon").getValue<string>())));
				games.push_back(game);				
			}	
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//					DESIGN DATA
		//
		////////////////////////////////////////////////////////////////////////////

		void parseDesignData()
		{
			JsonTree designDataJSON	= JsonTree(loadFile(getDesignDataPath()));			
			JsonTree designs = designDataJSON.getChild("designs");			
		
			for(auto it : designs)
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

		bool findGameId(int id, std::vector<int> gamesTurnOn)
		{
			for (auto it:gamesTurnOn)			
				if(it == id)
					return true;
			
			return false;
		}

		std::vector<GamesInfo> getGames()
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
		//				SET
		//
		////////////////////////////////////////////////////////////////////////////

		void setMenuConfigPath(const std::string& path)
		{
			menuConfigPath = path;
		}

		/*void setTuneUpConfigPath(const std::string& path)
		{
			tuneUpConfigPath = path;
		}	*/

		void setScreenSaverPath(const std::string& path)
		{
			screenSaverConfigPath =  path;
		}

		////////////////////////////////////////////////////////////////////////////
		//
		//				GET
		//
		////////////////////////////////////////////////////////////////////////////
		ConfigObject photoboothConfigObject, instakubConfigObject, mainConfigObject;

		const ConfigObject& getConfigObject(settings::id id)
		{
			if (id == settings::id::PHOTOBOOTH)
			{
				return photoboothConfigObject;
			}
			else if (id == settings::id::INSTAKUB)
			{
				return instakubConfigObject;
			}
			else if (id == settings::id::MAINCONFIG)
			{
				return mainConfigObject;
			}
		}

		std::string getLabelsPath()
		{
			return getFullPath(labelsPath);
		}

		std::string getMenuConfigPath()
		{
			return getFullPath(menuConfigPath);
		}

		/*std::string getTuneUpConfigPath()
		{
			return getFullPath(tuneUpConfigPath);
		}*/

		std::string getScreenSaverConfigPath()
		{
			return getFullPath(screenSaverConfigPath);
		}

		/*std::string getPhotoboothConfigPath()
		{
			return getFullPath(photoboothConfigPath);
		}*/

		std::string getFuncesConfigPath()
		{
			return getFullPath(funcesConfigPath);
		}

		std::string getInstagramConfigPath()
		{
			return getFullPath(instagramConfigPath);
		}

		std::string getKotopozaConfigPath()
		{
			return getFullPath(kotopozaConfigPath);
		}

		std::string getUserDataPath()
		{
			return getFullPath(userDataPath);
		}

		std::string getDesignDataPath()
		{
			return getFullPath(designDataPath);
		}

		DesignData getDesignData()
		{
			return designData;
		}

		int getUserDesignID()
		{
			return userDesignID;
		}		

		std::string getFullPath(string path)
		{
			return  getAppPath().string() + path;
		}

		std::string getLang()
		{
			return lang;
		}

	private:		
		int	   standID;
		bool   netConnection;		
		std::string userID;
		std::string lang;		
		std::string menuConfigPath;
		std::string labelsPath;		
		std::string designDataPath;		
		//std::string tuneUpConfigPath;
		std::string screenSaverConfigPath;
		std::string photoboothConfigPath;
		std::string funcesConfigPath;
		std::string instagramConfigPath;
		std::string kotopozaConfigPath;
		std::string userDataPath;

		GameId defaultGameID;
		DesignData designData;
		int userDesignID;

		fs::path getConfigPath()
		{		
			return getAppPath() / "data/configs/app.txt";
		}

		std::vector<GamesInfo> games;
	};

	typedef std::shared_ptr<ApplicationModel> ApplicationModelRef;	
}