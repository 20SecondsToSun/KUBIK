#include "TextItem.h"

using namespace kubik;
using namespace std;

void TextItem::setText(const string& text)
{
	this->text = text;
}

string TextItem::getText() const
{
	return text;
}

void TextItem::setFontName(const string& fontName)
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

void TextItem::setColor(const string& color)
{
	this->color = ci::ColorA::hex(stoi(color, 0, 16));
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