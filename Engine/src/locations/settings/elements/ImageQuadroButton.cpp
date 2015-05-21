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
	gl::Texture icon = item.getIcon();

	gl::draw(icon);

	if (selection && overtex)
	{
		gl::pushMatrices();
		gl::translate(-7, -7);
		gl::draw(overtex);
		gl::popMatrices();
	}

	gl::draw(textTex, Vec2f((icon.getWidth() - textTex.getWidth()) * 0.5f, icon.getHeight() + 10));
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