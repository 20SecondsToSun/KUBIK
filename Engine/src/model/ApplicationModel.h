#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace std;

class ApplicationModel
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

private:
	string userID;
	int standID;
	int defaultGameID;
	bool netConnection;

	int currentGame;

	vector<int> gameIDsAvailable;
	vector<int> gameIDsPurchased;
	vector<int> gameIDsTurnOn;
};