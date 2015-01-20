#pragma once

#include "IDrawable.h"
#include "Button.h"
#include "TextTools.h"
#include "settings/StatisticEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ToolRoundBtn> ToolRoundBtnRef;

		class ToolRoundBtn: public Button
		{
		public:
			ToolRoundBtn(string text, Font font, Color color = Color::white()):Button(Rectf(0,0,0,0)),
				text(text), font(font), color(color)
			{
				tex = textTools().getTextField(text, &font, color);
			}

			virtual void draw()
			{
				gl::draw(tex, Vec2f(buttonArea.x1 + 0.5 * (getWidth() - tex.getWidth()), buttonArea.y1 + 8));
				gl::color(color);
				gl::drawStrokedRoundedRect(buttonArea, 8, 200);
				gl::color(Color::white());				
			}

			void setFont(Font font)
			{
				this->font = font;
			}

			void setText(string text)
			{
				this->text = text;
			}

			void setColor(Color color)
			{
				this->color = color;
			}

		private:
			string text;
			Font font;
			Color color;
		};	
	}
}