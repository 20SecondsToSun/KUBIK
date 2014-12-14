#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ILocation.h"
#include "IPhotoboothLocation.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class PhotoTimer:public IPhotoboothLocation
	{
		Texture fon;
		ci::Font font;

	public:	
		~PhotoTimer(){};

		PhotoTimer(shared_ptr<PhotoboothSettings> settings)
		{
			reset(settings);
		}

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings = _settings;
			fon = settings->getTextures()["fon3"]->get();
			font  =  settings->getFonts()["helvetica40"]->get();
		}

		void draw()
		{
			gl::draw(fon, getWindowBounds());
			textTools().textFieldDraw("“¿…Ã≈–", &font, Vec2f(100, 100), Color::white());
		}

		void mouseUpHandler( Vec2i vec)
		{
			nextLocationSignal();
		}
	};
}