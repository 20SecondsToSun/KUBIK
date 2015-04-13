#pragma once
#include "cinder/app/AppNative.h"
using namespace std;

namespace instagram
{   
    class User
    {
	private:
		string id;
		string full_name;
		string profile_picture;
		string username;
	public:
		User()
		{

		};

		void parse(ci::JsonTree tree)
		{
			id				= tree.getChild("id").getValue<std::string>();
			full_name		= tree.getChild("full_name").getValue<std::string>();
			profile_picture = tree.getChild("profile_picture").getValue<std::string>();
			username		= tree.getChild("username").getValue<std::string>();
		}

		string getID() const
		{
			return id;
		}

	};
}
