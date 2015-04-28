#include "ImageSequencer\ImageSequencer.h"

using namespace kubik;

ImageSequencer::ImageSequencer(const std::vector<ci::gl::Texture>& images) :index(0), images(images)
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


float ImageSequencer::getHeight()
{
	return images[0].getHeight();
}

float ImageSequencer::getWidth()
{
	return images[0].getWidth();
}