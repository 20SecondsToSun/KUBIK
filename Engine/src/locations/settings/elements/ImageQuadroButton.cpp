#include "ImageQuadroButton.h"

using namespace kubik;
using namespace kubik::config;

ImageQuadroButton::ImageQuadroButton(OneDesignItem item, const ci::gl::Texture& overtex, const ci::Vec2f& pos)
	:SimpleSpriteButton(item.getIcon().getSize(), pos), item(item), selection(false), overtex(overtex)
{
	textTex = textTools().getTextField(item.getTextItem());
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
		gl::color(Color::hex(0xffff00));
	}

	gl::draw(textTex, Vec2f((item.getIcon().getWidth() - textTex.getWidth()) * 0.5f, item.getIcon().getHeight() + 10));
	gl::color(Color::white());
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