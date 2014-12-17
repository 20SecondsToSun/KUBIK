#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "cinder/gl/Texture.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class PhotoTemplate:public IPhotoboothLocation
	{
		Texture fon, stickerTex;
		ci::Font font;
	
		public:

		PhotoTemplate(shared_ptr<PhotoboothSettings> settings)
		{
			reset(settings);		
		};

		~PhotoTemplate()
		{

		};

		void start() override
		{
			console()<<"start PhotoTemplate"<<endl;
		}

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings	= _settings;
			fon			= settings->getTextures()["fon1"]->get();
			font		= settings->getFonts()["helvetica40"]->get();
			stickerTex  = settings->getActiveStickerTex();
		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			gl::color(Color::white());
			textTools().textFieldDraw("бшаепхре ьюакнм", &font, Vec2f(100.0f, 100.0f), Color::white());

			if(stickerTex)
				gl::draw(stickerTex, Vec2f(100.0f, 300.0f));
		}

		void mouseUpHandler( Vec2i vec) override
		{
			nextLocationSignal();
		}
	};
}