#include "DesignData.h"
using namespace kubik;

void OneDesignItem::setID(int id)
{
	this->id = id;
}

int OneDesignItem::getID() const
{
	return id;
}

TextItem OneDesignItem::getTextItem() const
{
	return text;
}

void OneDesignItem::setTextItem(const std::string& name, const std::string& fontName, float size, const std::string& color)
{
	text.setText(name);
	text.setFontName(fontName);
	text.setSize(size);
	text.setColor(color);
}

void OneDesignItem::setIconPath(const std::string& path)
{
	iconPath = path;
}

ci::gl::Texture OneDesignItem::getIcon() const
{
	return icon;
}

const std::string& OneDesignItem::getIconPath()
{
	return iconPath;
}

std::string OneDesignItem::getIconTexName() const
{
	return iconTextName;
}

void OneDesignItem::setIconTexName(const std::string& name)
{
	iconTextName = name;
}

std::string OneDesignItem::getDesignTexName()
{
	return designTexName;
}

void OneDesignItem::setDesignTexName(const std::string& name)
{
	designTexName = name;
}

void OneDesignItem::setIcon(const ci::gl::Texture& icon)
{
	this->icon = icon;
}

void OneDesignItem::setDesignTexture(const ci::gl::Texture& designTexture, const vector<ci::RectT<int>>& coordRect)
{
	for (size_t i = 0; i < coordRect.size(); i++)
		designData.push_back(sliceMappedTexture(designTexture, coordRect[i]));
}

void OneDesignItem::setDesignTexture(const ci::gl::Texture& designTexture)
{
	designData.push_back(designTexture);
}

ci::gl::Texture OneDesignItem::sliceMappedTexture(const ci::gl::Texture& tex, const ci::RectT<int>& rect)
{
	console() << "REFACTOR BIG HERE!!!!!!!!!!!!" << endl;
	static gl::Fbo fbo = gl::Fbo(rect.getWidth(), rect.getHeight());
	return fbo.getTexture();
	console() << "fbo size::  " << fbo.getSize() << endl;

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

std::vector<ci::gl::Texture> OneDesignItem::getMappedTextures()
{
	return this->designData;
}

void OneDesignItem::setFont(FontResourceDictionary fontDic)
{
	text.setFont(fontDic);
}

void OneDesignItem::setDesignPath(const std::string& path)
{
	designPath = path;
}

const std::string& OneDesignItem::getDesignPath() const
{
	return designPath;
}