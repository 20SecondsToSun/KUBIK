#pragma once

#include "TextItem.h"

namespace kubik
{	
	class JsonTools
	{
		public:
			static JsonTools& getInstance()
			{ 
				static JsonTools jtools; 
				return jtools; 
			};	

			config::TextItem parseTextItem(ci::JsonTree json);
	};

	inline JsonTools& jtools() 
	{
		return JsonTools::getInstance();
	};
}
