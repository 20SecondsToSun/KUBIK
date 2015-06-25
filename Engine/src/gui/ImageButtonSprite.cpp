#include "ImageButtonSprite.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;
using namespace kubik;

ImageButtonSprite::ImageButtonSprite(const ci::gl::Texture& image, const ci::Vec2f& pos)
	:SimpleSpriteButton(image.getWidth(), image.getHeight(), pos), image(image)
{

}

void ImageButtonSprite::drawLayout()
{
	gl::color(ci::Color::white());
	gl::color(ColorA(color.r, color.g, color.b, alpha));
	gl::translate(animPosition);
	gl::draw(image, ci::Vec2f(0.0f, 3.0f));
	gl::color(ci::Color::white());
}

void ImageButtonSprite::changeTexture(const ci::gl::Texture& image)
{
	this->image = image;
}