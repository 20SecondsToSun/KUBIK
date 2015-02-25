#pragma once

#include "TextTools.h"
#include "gui/SimpleSpriteButton.h"
#include "settings/StatisticEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ToolRoundBtn> ToolRoundBtnRef;

		class ToolRoundBtn: public SimpleSpriteButton
		{
		public:
			ToolRoundBtn(const ci::Rectf& rect, const std::string& text, ci::Font font, ci::Color color = ci::Color::white());
			
			virtual void drawLayout();
			void setAlpha(float alpha);
			void setText(const std::string& text);
			void setColor(const ci::Color& color);

		private:
			std::string text;
			ci::Font font;
			ci::ColorA color, bckColor;
			ci::gl::Texture tex;
		};	
	}
}