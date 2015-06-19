#include "VideoScreenSaver.h"
using namespace kubik;
using namespace ci;

VideoScreenSaver::VideoScreenSaver(const qtime::MovieGl& movie)
{
	this->movie = movie;
}

void VideoScreenSaver::draw()
{
	gl::Texture  texture = movie.getTexture();
	if (texture)
		gl::draw(texture, app::getWindowBounds());
}

void VideoScreenSaver::start()
{
	movie.setLoop(true, false);
	movie.play();
}

void VideoScreenSaver::stop()
{
	movie.stop();
}