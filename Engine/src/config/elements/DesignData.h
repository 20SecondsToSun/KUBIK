#pragma once

#include "TextItem.h"
#include "Utils.h"

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

		void setDesignTexture(const ci::gl::Texture& designTexture, const vector<ci::RectT<int>>& coordRect)
		{
			//this->designTexture = designTexture;
			for (size_t i = 0; i < coordRect.size(); i++)
			{
				designData.push_back(sliceMappedTexture(designTexture, coordRect[i]));
			}		
		}

		void setDesignTexture(const ci::gl::Texture& designTexture)
		{
			designData.push_back(designTexture);
		}

		ci::gl::Texture sliceMappedTexture(const ci::gl::Texture& tex, const ci::RectT<int>& rect)
		{
			gl::Fbo fbo = gl::Fbo(rect.getWidth(), rect.getHeight());

			Utils::drawGraphicsToFBO(fbo, [&]()
			{
				gl::pushMatrices();			
				gl::translate(-rect.x1, -rect.y1);				
				gl::draw(tex);
				gl::popMatrices();
			});

			gl::Texture retTex = fbo.getTexture();
			Utils::clearFBO(fbo);

			return retTex;
		}

		std::vector<ci::gl::Texture> getMappedTextures()
		{
			return this->designData;
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
		ci::gl::Texture icon;	
		std::string iconPath, designPath;	
		std::string iconTextName, designTexName;

		std::vector<ci::gl::Texture> designData;
	};

	typedef std::list<OneDesignItem> DesignData;
}