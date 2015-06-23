#include "preloader/Preloader.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace kubik;

Preloader::Preloader(const ci::Vec2f& position)
{
	create(position);
}

Preloader::Preloader()
{
	create(Vec2f(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f));	
}

void Preloader::create(const ci::Vec2f& position)
{
	this->position = position;
	auto path = (getAppPath() / "kubik/design_elements/preloader/").string();

	auto videoPath = fileTools().getVideoPath(path);

	if (!videoPath.empty())
	{
		qtime::MovieGl movie = qtime::MovieGl(videoPath);
		preloader = VideoPlayerRef(new VideoPlayer(movie));
		preloader->setPosition(0.5f * (getWindowSize() - movie.getSize()));
	}
	else	
	{
		auto files = fileTools().getAllImagePaths(path);

		if (!files.empty())
		{
			std::vector<ci::gl::Texture> preloaderSeq;
			for (size_t i = 0; i < files.size(); i++)
				preloaderSeq.push_back(gl::Texture(loadImage(ci::loadFile(files[i]))));

			preloader = ImageSequencerRef(new ImageSequencer(preloaderSeq));
			preloader->setPosition(0.5f * (getWindowSize() - preloaderSeq[0].getSize()));
		}		
	}
}

void Preloader::draw()
{
	if (background)
	{
		gl::draw(background);
		gl::color(ColorA(.0f, .0f, .0f, 0.8f));
		gl::drawSolidRect(getWindowBounds());

		gl::color(Color::white());
	}

	preloader->draw();
}

void Preloader::setPosition(const ci::Vec2f& position)
{
	this->position = position;
}

void Preloader::init(ISettingsRef config)
{

}

void Preloader::reset() 
{

}

void Preloader::set(IMovieRef value)
{
	preloader = value;
	preloader->setPosition(0.5f * (getWindowSize() - Vec2f(preloader->getWidth(), preloader->getHeight())));
}

void Preloader::setBackground(const gl::Texture& texture)
{
	background = texture;
}

void Preloader::setTextures()
{

}