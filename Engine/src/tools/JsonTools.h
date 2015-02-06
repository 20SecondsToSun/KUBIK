#pragma once

#include "TextItem.h"

namespace kubik
{	
	class JsonTools
	{
	public:

		static JsonTools& getInstance() { 
			static JsonTools jtools; 
			return jtools; 
		};	

		TextItem parseTextItem(ci::JsonTree json)
		{
			TextItem textItem;
			textItem.setText(json.getChild("text").getValue<string>());
			textItem.setFontName(json.getChild("font").getValue<string>());
			textItem.setSize(json.getChild("size").getValue<int>());
			textItem.setColor(json.getChild("color").getValue<string>());
			return textItem;
		}
	};
	// helper function(s) for easier access 
	inline JsonTools& jtools() {return JsonTools::getInstance();};
}