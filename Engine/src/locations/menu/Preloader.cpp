#include "preloader/Preloader.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace kubik;

Preloader::Preloader(const ci::Vec2f& position)
{
	create(position);
	
	/*preloaderMovie = qtime::MovieGl(loadAsset("test.gif"));
	preloaderMovie.setLoop();
	preloaderMovie.play();*/
}

Preloader::Preloader()
{
	create(Vec2f(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f));	
}

void Preloader::create(const ci::Vec2f& position)
{
	this->position = position;

	std::vector<std::string> files = fileTools().getAllJpegPaths(getAssetPath(fs::path("preloader\\")).string());
	std::vector<ci::gl::Texture> preloaderSeq;

	for (int i = 0; i < files.size(); i++)
		preloaderSeq.push_back(gl::Texture(loadImage(ci::loadFile(files[i]))));	

	sequencer = ImageSequencerRef(new ImageSequencer());
	sequencer->setImages(preloaderSeq);	
	sequencer->setPosition(0.5f * (getWindowSize() - preloaderSeq[0].getSize()));
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

	sequencer->draw();

}

void Preloader::setPosition(const ci::Vec2f& position)
{
	this->position = position;
}

void Preloader::init(ISettingsRef config)
{

}

void Preloader::reset() {}

void Preloader::setBackground(const gl::Texture& texture)
{
	background = texture;
}

void Preloader::setTextures()
{

}