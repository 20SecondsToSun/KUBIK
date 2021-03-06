#pragma once

#include "EventGUI.h"
#include "gui/SimpleSpriteButton.h"
#include "TextTools.h"

namespace kubik
{
	namespace gui
	{
		typedef std::shared_ptr<class TextField> TextFieldRef;

		class TextField : public kubik::SimpleSpriteButton
		{
			std::string text;
			ci::Rectf field;			
			ci::Color color;
			ci::Font font;
			bool touched;

			ci::gl::Texture inputFieldTexture;

			ci::Anim<ci::Color> borderColor;

			void drawCarriage();
			bool carridgePhase;

		public:
			TextField(const ci::Rectf& value);

			void addLetter(const char& letter);
			void addLetter(const std::string& letter);
			void clear();
			void setTouched(bool value);
			void setField(const ci::Rectf& field);
			void setFont(const ci::Font& font);
			void setTextColor(const ci::Color& color);
			void showThatEmpty();
			bool empty();
			void draw();					
			std::string getText();
		};
	}
}