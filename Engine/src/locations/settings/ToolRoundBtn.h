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
				text(text), font(font), color(color), bckColor(Color::white())
			{
				tex = textTools().getTextField(text, &font, color);
			}

			virtual void draw()
			{				
				gl::color(bckColor);	
				gl::draw(tex, Vec2f(buttonArea.x1 + 0.5 * (getWidth() - tex.getWidth()), buttonArea.y1 + 8));
				gl::color(color);
				gl::drawStrokedRoundedRect(buttonArea, 8, 200);
				gl::color(Color::white());				
			}

			void setAlpha(float  alpha)
			{
				color    = ColorA(color.r, color.g, color.b, alpha);
				bckColor = ColorA(bckColor.r, bckColor.g, bckColor.b, alpha);
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
				tex = textTools().getTextField(text, &font, color);
			}

		private:
			string text;
			Font font;
			ColorA color, bckColor;
		};	
	}
}