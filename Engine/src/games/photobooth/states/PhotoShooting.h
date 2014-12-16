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
	class PhotoShooting:public IPhotoboothLocation
	{
		Texture fon;
		ci::Font font;

		public:

		PhotoShooting(shared_ptr<PhotoboothSettings> settings)
		{
			reset(settings);		
		};

		~PhotoShooting()
		{

		};

		void start() override
		{
			console()<<"start PhotoShooting"<<endl;
		}

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings = _settings;
			fon  = settings->getTextures()["fon1"]->get();
			font =  settings->getFonts()["helvetica40"]->get();
		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			gl::color(Color::white());
			textTools().textFieldDraw("ÔÎÒÎÃÐÀÔÈÐÓÅÌ", &font, Vec2f(100.0f, 100.0f), Color::white());
			textTools().textFieldDraw("ÊÎËÈ×ÅÑÒÂÎ ÑÍÈÌÊÎÂ " + to_string(settings->getData().getPhotoShots()), &font, Vec2f(100.0f, 200.0f), Color::white());
			textTools().textFieldDraw("ÂÐÅÌß ÎÄÍÎÃÎ ÑÍÈÌÊÀ(ÑÅÊ.) " + to_string(settings->getData().secondsBetweenShots), &font, Vec2f(100.0f, 300.0f), Color::white());
		}

		void mouseUpHandler( Vec2i vec) override
		{
			nextLocationSignal();
		}
	};
}