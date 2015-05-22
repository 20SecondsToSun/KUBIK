#include "ImageQuadroButton.h"

using namespace kubik;
using namespace kubik::config;

ImageQuadroButton::ImageQuadroButton(const SixButtonOneData& data) :SimpleSpriteButton(data.getDesignItem().getIcon().getSize(), data.getPosition()),
item(data.getDesignItem()), selection(false), overtex(data.getOverTexture()), backgroundTextColor(data.getBgTextColor())
{
	textPosition = Vec2f(0, item.getIcon().getHeight() + 10.0f);
	textBoxSize = Vec2i(item.getIcon().getWidth(), 50.0f);
}

void ImageQuadroButton::drawLayout()
{
	gl::draw(item.getIcon());

	if (selection && overtex)
	{
		gl::pushMatrices();
		gl::translate(-7, -7);
		gl::draw(overtex);
		gl::popMatrices();	
		textTools().drawTextBox(item.getTextItem(), backgroundTextColor, Color::hex(0xffff00), textPosition, textBoxSize);
	}
	else
		textTools().drawTextBox(item.getTextItem(), backgroundTextColor, textPosition, textBoxSize);
}

void ImageQuadroButton::setAlpha(float alpha)
{
}

void ImageQuadroButton::setSelection(bool value)
{
	selection = value;
}

void ImageQuadroButton::swapSelection()
{
	selection = !selection;
}

OneDesignItem ImageQuadroButton::getItem() const
{
	return item;
}