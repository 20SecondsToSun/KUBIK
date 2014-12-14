#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ILocation.h"
#include "PhotoboothSettings.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class PhotoFilter:public IPhotoboothLocation
	{		
		Texture fon;
		ci::Font font;

	public:	

		PhotoFilter(shared_ptr<PhotoboothSettings> settings)
		{
			reset(settings);
		}

		~PhotoFilter()
		{
			console()<<"photo filter destructor"<<endl;
		}		

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings = _settings;
			fon = settings->getTextures()["fon2"]->get();
			font		   =  settings->getFonts()["helvetica40"]->get();
		}

		void draw()
		{
			gl::draw(fon, getWindowBounds());
			textTools().textFieldDraw("¬€¡Œ– ‘»À‹“–¿", &font, Vec2f(100, 100), Color::white());
		}

		void mouseUpHandler( Vec2i vec)
		{
			nextLocationSignal();
		}
	};
}