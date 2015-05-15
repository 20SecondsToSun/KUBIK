#include "gui/TextField.h"

using namespace kubik;
using namespace kubik::gui;

TextField::TextField(const ci::Rectf& value) : SimpleSpriteButton(value), borderColor(Color::hex(0xe0e7ed))
{

}

void TextField::addLetter(const char& letter)
{

}

void TextField::addLetter(const std::string& letter)
{
	if (letter == "back")
	{
		if (!text.empty())
		{
			text.pop_back();
			inputFieldTexture = textTools().getTextField(text, &font, color);
		}
	}
	else if (inputFieldTexture.getWidth() < SimpleSpriteButton::getWidth() - 20.0f)
	{
		text += letter;
		inputFieldTexture = textTools().getTextField(text, &font, color);
	}
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

void TextField::showThatEmpty()
{
	Color col = Color::hex(0xe0e7ed);
	timeline().apply(&borderColor, Color(1.0f, 0.0f, 0.0f), Color(col.r, col.g, col.b), 0.9f, EaseOutCubic());
}

bool TextField::empty()
{
	return text.empty();
}

std::string TextField::getText()
{
	return text;
}

void TextField::draw()
{
	gl::pushMatrices();
	gl::translate(- Vec2f(5.0f, 0.0f));
	if (touched || !text.empty())	
		SimpleSpriteButton::draw();	
	gl::popMatrices();
	
	gl::pushMatrices();
	gl::color(borderColor);
	gl::color(ColorA(borderColor.value().r, borderColor.value().g, borderColor.value().b, alpha));
	gl::translate(SimpleSpriteButton::getGlobalPosition() - Vec2f(5.0f, 0.0f));
	gl::drawStrokedRect(SimpleSpriteButton::getButtonArea());
	gl::popMatrices();
	gl::color(ColorA(1, 1, 1, alpha));
	gl::draw(inputFieldTexture, SimpleSpriteButton::getGlobalPosition() + Vec2f(5.0f, 3.0f));

	drawCarriage();
}

void TextField::drawCarriage()
{
	auto position = SimpleSpriteButton::getGlobalPosition() + Vec2f(5.0f, 3.0f);
	auto x1 = position.x + inputFieldTexture.getWidth();
	auto x2 = position.x + inputFieldTexture.getWidth() + 2.0f;
	auto y1 = position.y;
	auto y2 = position.y + inputFieldTexture.getHeight();

	if (touched)
	{
		if (getElapsedFrames() % 30 == 0)
			carridgePhase = !carridgePhase;

		if (carridgePhase)
		{
			gl::color(color);
			gl::drawSolidRect(Rectf(x1, y1, x2, y2));
		}
	}
}