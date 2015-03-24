#pragma once
#include "Types.h"

namespace kubik
{
	class TextItem
	{
	public:
		TextItem()
		{

		}

		void setText(const std::string& text)
		{
			this->text = text;
		}
	
		std::string getText() const
		{
			return text;
		}

		void setFontName(const std::string& fontName)
		{
			this->fontName = fontName;
		}

		ci::Font getFont() const
		{
			return font;
		}

		void setSize(float size)
		{
			this->size = size;
		}

		void setColor(const std::string& color)
		{			
			this->color = ci::ColorA::hex(std::stoi(color, 0, 16));
		}

		void setColor(const ci::ColorA& color)
		{			
			this->color = color;
		}


		ci::ColorA getColor() const
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