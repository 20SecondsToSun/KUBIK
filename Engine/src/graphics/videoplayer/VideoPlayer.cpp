#include "videoplayer\VideoPlayer.h"

using namespace kubik;
using namespace ci;

VideoPlayer::VideoPlayer(const qtime::MovieGl& video)
	:video(video)
{
	this->video.setLoop();
	this->video.play();	
}

void VideoPlayer::draw()
{
	if (video)
	{
		auto videoTexture = video.getTexture();
		if (videoTexture)
		{
			gl::pushMatrices();
			gl::translate(position);
			gl::draw(videoTexture);
			gl::popMatrices();
		}	
	}
}

float VideoPlayer::getHeight()
{
	return video.getTexture().getHeight();
}

float VideoPlayer::getWidth()
{
	return video.getTexture().getWidth();
}
