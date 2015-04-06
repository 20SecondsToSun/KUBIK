#pragma once
#include "cinder/app/AppNative.h"
#include "instagram/api/InstBaseObject.h"

namespace instagram
{   
    class Pagination : public InstBaseObject
    {
		std::string next_url;
		bool _hasNextURL;

	public:
		std::string getNextURL() const 
		{
			return  next_url;
		}

		void setNextURL(const std::string& value)
		{
			next_url = value;
		}

		bool hasNextURL() const
		{
			return _hasNextURL;
		}

		void parse(ci::JsonTree tree)
		{
			_hasNextURL = true;

			try
			{
				next_url = tree.getChild("next_url").getValue<std::string>();				
			}
			catch (...)
			{
				next_url = "";
				_hasNextURL = false;
			}

			//ci::app::console()<<"next_url :: "<<next_url<<std::endl;
		}
	};
}