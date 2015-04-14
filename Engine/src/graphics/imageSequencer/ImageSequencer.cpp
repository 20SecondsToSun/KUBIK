#include "ImageSequencer\ImageSequencer.h"

using namespace kubik;

ImageSequencer::ImageSequencer() :index(0)
{

}

void ImageSequencer::draw()
{
	gl::pushMatrices();
	gl::translate(position);
	gl::draw(images[index++]);

	if (index >= images.size())
		index = 0;
	gl::popMatrices();
}

void ImageSequencer::setImages(const std::vector<ci::gl::Texture>& images)
{
	this->images = images;
}

void ImageSequencer::setPosition(const ci::Vec2f& vec)
{
	position = vec;
}