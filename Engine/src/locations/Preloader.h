#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Resources.h"
#include "cinder/gl/Texture.h"
#include "IScreen.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class Preloader:public IScreen
	{
	public:
		Preloader(Vec2f position)
		{
			this->position = position;
			tex = gl::Texture ( loadImage( ci::app::App::loadResource( 2, "IMAGE" ) ) );
		}

		Preloader()
		{
			this->position = Vec2f(getWindowWidth() * 0.5, getWindowHeight() * 0.5);
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

		void setPosition(Vec2f position)
		{
			this->position = position;
		}

		void init(shared_ptr<ISettings> config) override
		{

		}

		void reset(shared_ptr<ISettings> config) override{};

	protected:
		void setTextures()
		{

		}

	private:
		Vec2f position;
		Texture tex;
	};
}