#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Resources.h"
#include "cinder/gl/Texture.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class Preloader
{
public:
	Preloader(Vec2f position)
	{
		this->position = position;
		tex = gl::Texture ( loadImage( ci::app::App::loadResource( 2, "IMAGE" ) ) );
	}

	void draw()
	{
		gl::pushMatrices();
		gl::translate(position);
		gl::scale( 0.5f, 0.5f );
		gl::rotate( 180.0f * float( getElapsedSeconds() ) );
		gl::translate( -0.5f * Vec2f(151.0f, 151.0f ) );
		gl::draw(tex);
		gl::popMatrices();
	}

private:
	Vec2f position;
	Texture tex;
};