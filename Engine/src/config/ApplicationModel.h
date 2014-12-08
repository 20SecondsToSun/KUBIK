#pragma once
#include "cinder/app/AppNative.h"
#include <boost/algorithm/string.hpp>
#include "KubikException.h"
#include "ApplicationModel.h"
#include "IConfig.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class ApplicationModel: public IConfig
	{

	public:		

		ApplicationModel()
		{
			
		}

		void load()
		{
			try	
			{
				JsonTree configJSON		= JsonTree(loadFile(getConfigPath()));
				userID					= configJSON.getChild("userID").getValue<string>();
				standID					= configJSON.getChild("standID").getValue<int>();
				netConnection			= configJSON.getChild("netConnection").getValue<bool>();

				menuConfigPath			= configJSON.getChild("menuConfigPath").getValue<string>();
				tuneUpConfigPath		= configJSON.getChild("tuneUpConfigPath").getValue<string>();
				screenSaverConfigPath	= configJSON.getChild("screenSaverConfigPath").getValue<string>();

				defaultGameID			= configJSON.getChild("defaultGameID").getValue<int>();					

				vector<int> temp;

				JsonTree gamesAvailable = JsonTree(configJSON.getChild( "gamesAvailable"));
				for(auto it : gamesAvailable)
					temp.push_back(it.getValue<int>());

				gameIDsAvailable = temp;
				temp.clear();


				JsonTree gamesPurchased = JsonTree(configJSON.getChild( "gamesPurchased"));
				for(auto it : gamesPurchased)
					temp.push_back(it.getValue<int>());

				gameIDsPurchased = temp;
				temp.clear();


				JsonTree gamesTurnOn = JsonTree(configJSON.getChild( "gamesTurnOn"));
				for(auto it : gamesTurnOn)
					temp.push_back(it.getValue<int>());

				gameIDsTurnOn = temp;
				temp.clear();		
			}
			catch(...)
			{
				throw ExcConfigFileParsing();
			}			
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

		// set paths //

		void setMenuConfigPath(string path)
		{
			menuConfigPath = getAppPath().string() + path;
		}

		void setTuneUpConfigPath(string path)
		{
			tuneUpConfigPath = getAppPath().string() + path;
		}	

		void setScreenSaverConfigPath(string path)
		{
			screenSaverConfigPath = getAppPath().string() + path;
		}

		string getMenuConfigPath()
		{
			return menuConfigPath;
		}

		string getTuneUpConfigPath()
		{
			return tuneUpConfigPath;
		}

		string getScreenSaverConfigPath()
		{
			return screenSaverConfigPath;
		}


	private:

		string userID;
		int standID;
		int defaultGameID;
		bool netConnection;	

		string menuConfigPath;
		string tuneUpConfigPath;
		string screenSaverConfigPath;

		vector<int> gameIDsAvailable;
		vector<int> gameIDsPurchased;
		vector<int> gameIDsTurnOn;	

		fs::path getConfigPath()
		{		
			return getAppPath() / "data/configs/app.txt";
		}
	};
}