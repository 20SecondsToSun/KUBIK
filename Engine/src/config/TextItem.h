#pragma once
//#include "TextTools.h"
//#include "ConfigSettings.h"

namespace kubik
{
	class TextItem
	{
	public:
		/*TextItem(std::string text, std::string font, float size, ci::ColorA color)
			:text(text),
			font(font),
			size(size), 
			color(color)
		{

		}*/

		TextItem()
		{

		}

		void setText(const std::string& text)
		{
			this->text = text;
		}
	
		std::string getText()
		{
			return text;
		}

		void setFontName(std::string fontName)
		{
			this->fontName = fontName;
		}

		ci::Font getFont()
		{
			return font;
		}

		void setSize(float size)
		{
			this->size = size;
		}

		void setColor(string color)
		{			
			this->color = ci::ColorA::hex(std::stoi(color, 0, 16));
		}

		void setColor(ci::ColorA color)
		{			
			this->color = color;
		}


		ci::ColorA getColor()
		{
			return color;
		}

		void setFont(FontResourceDictionary fontDic)
		{
			font = fontDic[fontName + to_string(size)]->get();
		}

		void draw()
		{

		}

	private:
		std::string text;

		std::string fontName;
		int size;
		ci::ColorA color;
		ci::Font font;
	};

	typedef std::shared_ptr<TextItem> TextItemRef;
}