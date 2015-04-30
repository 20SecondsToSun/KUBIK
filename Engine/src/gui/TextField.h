#pragma once
#include "EventGUI.h"
#include "gui/SimpleSpriteButton.h"
#include "TextTools.h"

namespace kubik
{
	namespace gui
	{
		typedef shared_ptr<class TextField> TextFieldRef;

		class TextField : public kubik::SimpleSpriteButton
		{
			ci::Rectf field;
			std::string text;
			ci::Color color;
			ci::Font font;
			bool touched;

		public:
			TextField(const ci::Rectf& value);

			void addLetter(const char& letter);
			void addLetter(const std::string& letter);
			void clear();
			void setTouched(bool value);
			void setField(const ci::Rectf& field);
			void setFont(const ci::Font& font);
			void setTextColor(const ci::Color& color);
			void showEmpty();
			void draw();
			std::string getText();
		};
	}
}