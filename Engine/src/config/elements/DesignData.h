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

		std::string getIconTexName()
		{
			return iconTextName;//text.getText() + to_string(id);
		}

		void setIconTexName(const std::string& name)
		{
			iconTextName = name;
		}

		///

		std::string getDesignTexName()
		{
			return designTexName;//text.getText() + to_string(id);
		}

		void setDesignTexName(const std::string& name)
		{
			designTexName = name;
		}

		///

		void setIcon(const ci::gl::Texture& icon)
		{
			this->icon = icon;
		}

		void setDesignTexture(const ci::gl::Texture& icon)
		{
			this->designTexture = designTexture;
		}

		ci::gl::Texture getDesignTexture()
		{
			return this->designTexture;
		}		

		void setFont(FontResourceDictionary fontDic)
		{
			text.setFont(fontDic);
		}

		void setDesignPath(const std::string& path)
		{
			designPath = path;
		}	

		const std::string& getDesignPath()
		{
			return designPath;
		}			

	private:
		int id;
		TextItem text;
		ci::gl::Texture icon, designTexture;	
		std::string iconPath, designPath;	
		std::string iconTextName, designTexName;
	};

	typedef std::list<OneDesignItem> DesignData;
}