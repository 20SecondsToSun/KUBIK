#include "preloader/Preloader.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace kubik;

Preloader::Preloader(const ci::Vec2f& position)
{
	this->position = position;
	tex = gl::Texture(loadImage(ci::app::App::loadResource(2, "IMAGE")));
}

Preloader::Preloader()
{
	this->position = Vec2f(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f);
	tex = gl::Texture(loadImage(ci::app::App::loadResource(2, "IMAGE")));
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

	gl::pushMatrices();
	gl::translate(position);
	gl::scale(0.5f, 0.5f);
	gl::rotate(180.0f * float(getElapsedSeconds()));
	gl::translate(-0.5f * Vec2f(151.0f, 151.0f));
	gl::draw(tex);
	gl::popMatrices();
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