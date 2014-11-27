#pragma once

#include "cinder/app/AppNative.h"

using namespace std;

class ApplicationModel
{
	public:

		void setUserID(string _value)
		{
			userID = _value;
		}

		void setGameIDsPurshased(vector<int> _value)
		{
			gameIDsPurshased = _value;
		}
   
	private:
		string userID;
		vector<int> gameIDsPurshased;
};