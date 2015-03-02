#pragma once

#include "TextItem.h"

namespace kubik
{
	class OneDesignItem
	{
	public:
		OneDesignItem()
		{

		}

		void setID(int id)
		{
			this->id = id;
		}

		int getID()
		{
			return id;
		}
		
		TextItem getTextItem()
		{
			return text;
		}

		void setTextItem(std::string name, string fontName, float size, string color)
		{
			text.setText(name);
			text.setFontName(fontName);
			text.setSize(size);
			text.setColor(color);
		}		

		void setIconPath(const std::string& path)
		{
			iconPath = path;
		}

		ci::gl::Texture getIcon()
		{
			return icon;
		}

		const std::string& getIconPath()
		{
			return iconPath;
		}

		std::string getName()
		{
			return text.getText() + to_string(id);
		}

		void setIcon(ci::gl::Texture icon)
		{
			this->icon = icon;
		}	

		void setFont(FontResourceDictionary fontDic)
		{
			text.setFont(fontDic);
		}

	private:
		int id;
		TextItem text;
		ci::gl::Texture icon;	
		std::string iconPath;	

	};

	typedef std::list<OneDesignItem> DesignData;
}