#include "ImageScreenSaver.h"
using namespace kubik;
using namespace ci;

ImageScreenSaver::ImageScreenSaver(const gl::Texture& texture)
{
	this->texture = texture;
}

void ImageScreenSaver::draw()
{
	gl::draw(texture);
}

void ImageScreenSaver::start()
{

}

void ImageScreenSaver::stop()
{

}