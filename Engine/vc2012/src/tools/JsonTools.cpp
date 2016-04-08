#include "JsonTools.h"

using namespace kubik;

config::TextItem JsonTools::parseTextItem(ci::JsonTree json)
{
	config::TextItem textItem;
	textItem.setText(json.getChild("text").getValue<std::string>());
	textItem.setFontName(json.getChild("font").getValue<std::string>());
	textItem.setSize(json.getChild("size").getValue<int>());
	textItem.setColor(json.getChild("color").getValue<std::string>());

	return textItem;
}
