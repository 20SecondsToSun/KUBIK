#pragma once

#include "cinder/app/AppNative.h"

namespace instagram
{   
    class User
    {
	private:
		std::string id;
		std::string full_name;
		std::string profile_picture;
		std::string username;
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

		std::string getID() const
		{
			return id;
		}

		std::string getUsername() const
		{
			return username;
		}
	};
}
