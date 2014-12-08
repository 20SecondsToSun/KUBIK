#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "IModel.h"
#include "ISettings.h"
#include "FuncesSettings.h"
#include "PhotoboothSettings.h"
#include "Types.h"

using namespace std;
using namespace kubik;

class ApplicationModel: public IModel
{
public:

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


	////////////////////////////////////////////

	bool isGameCurrent(int id)
	{
		return currentGame == id;
	}

	int getCurrentGame()
	{
		return currentGame;
	}

	void setCurrentGame(int id)
	{
		currentGame =  id;
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

	/////////////////////////////////////////

	bool onlyOneGameOn()
	{
		return gameIDsTurnOn.size() == 1;
	}


	ISettings* getGameSettingsById()
	{
		ISettings *config;
	
		switch (currentGame)
		{
		case gameId::PHOTOBOOTH:		
			config = photoBoothSettings;			
			break;

		case gameId::FUNCES:
			config = funcesSettings;
			break;

		default:
			break;
		}

		return config;
	}

	void setFuncesSettings(FuncesSettings* fs)
	{
		funcesSettings =  fs;
	}

	void setPhotoboothSettings(PhotoboothSettings* phs)
	{
		photoBoothSettings =  phs;
	}

	// set paths //

	void setMenuConfigPath(string path)
	{
		menuConfigPath = getAppPath().string() +path;
	}

	void setTuneUpConfigPath(string path)
	{
		tuneUpConfigPath = getAppPath().string() +path;
	}	

	void setScreenSaverConfigPath(string path)
	{
		screenSaverConfigPath = getAppPath().string() +path;
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
	int currentGame;

	string menuConfigPath;
	string tuneUpConfigPath;
	string screenSaverConfigPath;
	

	vector<int> gameIDsAvailable;
	vector<int> gameIDsPurchased;
	vector<int> gameIDsTurnOn;

	FuncesSettings *funcesSettings;
	PhotoboothSettings *photoBoothSettings;
};