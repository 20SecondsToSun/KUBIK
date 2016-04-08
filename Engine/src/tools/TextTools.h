#pragma once

#include "StringTools.h"
#include "TextItem.h"

namespace kubik
{
	class TextTools
	{
	public:
		static TextTools& getInstance() 
		{ 
			static TextTools tt; 
			return tt; 
		};

		ci::gl::Texture getTextField(const std::string& text, ci::Font* font, const ci::ColorA& color);
		ci::gl::Texture getTextField(const char* text, ci::Font* font, const ci::ColorA& color);
		ci::Surface getTextField(const config::TextItem& item, bool isCentered = false, float offset = 0);

		void textFieldDraw(const std::string& text, ci::Font *font, const ci::ColorA& color, const ci::Vec2f& coords = ci::Vec2f::zero());
		void textFieldDraw(const config::TextItem& item, const ci::Vec2f& coords = ci::Vec2f::zero());
		void drawTextBox(const config::TextItem& item, const ci::Color& color, const ci::Vec2f& position, const ci::Vec2f& size);
		void drawTextBox(const config::TextItem& item, const ci::Color& color, const ci::Color& textColor, const ci::Vec2f& position, const ci::Vec2f& size);
	};

	inline TextTools& textTools() 
	{
		return TextTools::getInstance(); 
	};
}
