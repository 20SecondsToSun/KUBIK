#pragma once
#include "Types.h"

namespace kubik
{
	class TextItem
	{
	public:
		void setText(const std::string& text);			
		void setFontName(const std::string& fontName);		
		void setSize(float size);
		void setColor(const std::string& color);
		void setColor(const ci::ColorA& color);
		void setFont(FontResourceDictionary fontDic);

		ci::Font getFont() const;
		ci::ColorA getColor() const;
		std::string getText() const;		

	private:
		int size;
		std::string text;
		std::string fontName;		
		ci::ColorA color;
		ci::Font font;
	};

	typedef std::shared_ptr<TextItem> TextItemRef;
}