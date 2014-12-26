#pragma once

class VideoScreenSaver:public IResourceScreenSaver
{
public:
	VideoScreenSaver(qtime::MovieGl movie)
	{
		this->movie = movie;
	}

	void draw()
	{
		gl::Texture  texture = movie.getTexture();
		if(texture)
			gl::draw(texture, getWindowBounds());
	}

	void start()
	{
		movie.setLoop( true, false );
		movie.play();
	}

	void stop()
	{
		movie.stop();
	}

private:
	qtime::MovieGl movie;
};