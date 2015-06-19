#include "TextItem.h"
using namespace kubik;

void TextItem::setText(const std::string& text)
{
	this->text = text;
}

std::string TextItem::getText() const
{
	return text;
}

void TextItem::setFontName(const std::string& fontName)
{
	this->fontName = fontName;
}

ci::Font TextItem::getFont() const
{
	return font;
}

void TextItem::setSize(float size)
{
	this->size = size;
}

void TextItem::setColor(const std::string& color)
{
	this->color = ci::ColorA::hex(std::stoi(color, 0, 16));
}

void TextItem::setColor(const ci::ColorA& color)
{
	this->color = color;
}

ci::ColorA TextItem::getColor() const
{
	return color;
}

void TextItem::setFont(FontResourceDictionary fontDic)
{
	font = fontDic[fontName + to_string(size)]->get();
}