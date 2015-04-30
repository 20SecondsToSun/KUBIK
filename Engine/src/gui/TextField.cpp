#include "gui/TextField.h"

using namespace kubik;
using namespace kubik::gui;

TextField::TextField(const ci::Rectf& value) :SimpleSpriteButton(value)
{
	
}

void TextField::addLetter(const char& letter)
{

}

void TextField::addLetter(const std::string& letter)
{
	text += letter;
	inputFieldTexture = textTools().getTextField(text, &font, color);
}

void TextField::clear()
{
	text.clear();
	inputFieldTexture = textTools().getTextField(text, &font, color);
}

void TextField::setTouched(bool value)
{
	touched = value;
}

void TextField::setField(const ci::Rectf& value)
{
	field = value;
}

void TextField::setFont(const ci::Font& value)
{
	font = value;
}

void TextField::setTextColor(const ci::Color& value)
{
	color = value;
}

void TextField::showEmpty()
{

}

std::string TextField::getText()
{
	return text;
}

void TextField::draw()
{
	if (touched)
	{
		SimpleSpriteButton::draw();
		//gl::color(Color(1, 0, 0));
		//gl::drawSolidRect(field);
		//gl::color(Color::white());
	}
	inputFieldTexture
}