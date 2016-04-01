#pragma once

#include "TextItem.h"
#include "Utils.h"

namespace kubik
{
	class OneDesignItem
	{
	public:
		void setID(int id);
		void setTextItem(const std::string& name, const std::string& fontName, float size, const std::string& color);
		void setIconPath(const std::string& path);
		void setDesignTexName(const std::string& name);
		void setIcon(const ci::gl::Texture& icon);
		void setDesignTexture(const ci::gl::Texture& designTexture, const vector<ci::RectT<int>>& coordRect);
		void setDesignTexture(const ci::gl::Texture& designTexture);
		void setFont(FontResourceDictionary fontDic);
		void setDesignPath(const std::string& path);
		void setIconTexName(const std::string& name);
		int getID() const;		

		TextItem getTextItem() const;			

		const std::string& getIconPath();
		const std::string& getDesignPath() const;
		std::string getIconTexName() const;		
		std::string getDesignTexName();

		ci::gl::Texture getIcon() const;
		ci::gl::Texture sliceMappedTexture(const ci::gl::Texture& tex, const ci::RectT<int>& rect);
		std::vector<ci::gl::Texture> getMappedTextures();

	private:
		int id;
		TextItem text;
		ci::gl::Texture icon;	
		std::string iconPath, designPath;	
		std::string iconTextName, designTexName;
		std::vector<ci::gl::Texture> designData;
	};

	typedef std::vector<OneDesignItem> DesignData;
}