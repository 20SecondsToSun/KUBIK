#include "ToolRoundBtn.h"

using namespace kubik;
using namespace kubik::config;
using namespace ci;
using namespace ci::app;

ToolRoundBtn::ToolRoundBtn(const ci::Rectf& rect, const std::string& text, ci::Font font, ci::Color color)
	:SimpleSpriteButton(rect), text(text), font(font), color(color), bckColor(Color::white())
{
	tex = textTools().getTextField(text, &font, color);
}

void ToolRoundBtn::drawLayout()
{					
	gl::color(bckColor);	
	gl::draw(tex, Vec2f(buttonArea.x1 + 0.5 * (getWidth() - tex.getWidth()), buttonArea.y1 + 8));
	gl::color(color);
	gl::drawStrokedRoundedRect(buttonArea, 8, 200);
	gl::color(Color::white());		
}

void ToolRoundBtn::setAlpha(float alpha)
{
	color	 = Utils::colorAlpha(color, alpha);
	bckColor = Utils::colorAlpha(bckColor, alpha);
}

void ToolRoundBtn::setText(const std::string& text)
{
	this->text = text;
}

void ToolRoundBtn::setColor(const ci::Color& color)
{
	this->color = color;
	tex = textTools().getTextField(text, &font, color);
}