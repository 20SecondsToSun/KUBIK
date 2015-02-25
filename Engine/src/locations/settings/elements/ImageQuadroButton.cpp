#include "ImageQuadroButton.h"

using namespace kubik;
using namespace kubik::config;

ImageQuadroButton::ImageQuadroButton(OneDesignItem item, const ci::Vec2f& pos)
	:SimpleSpriteButton(item.getIcon().getSize(), pos), item(item), selection(false)
{
	textTex = textTools().getTextField(item.getTextItem());				
}

void ImageQuadroButton::drawLayout()
{
	gl::Texture icon = item.getIcon();

	gl::draw(icon);			

	if(selection)
	{
		gl::color(Color::hex(0xffff00));
		gl::lineWidth(7);
		gl::drawStrokedRoundedRect(ci::Rectf(ci::Vec2f::zero(), icon.getSize()), 6);
		gl::lineWidth(1);					
	}

	gl::draw(textTex, Vec2f((icon.getWidth() - textTex.getWidth()) * 0.5, icon.getHeight() + 10));
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