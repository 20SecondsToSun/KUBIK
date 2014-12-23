#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ILocation.h"
#include "IPhotoboothLocation.h"
#include "CameraAdapter.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;
using namespace ci;

namespace kubik
{
	class PhotoTimer:public IPhotoboothLocation
	{
		Texture fon;
		Font font;
		Font font100;
		int seconds;
		Timer cdTimer;		

	public:

		void clear()
		{
			cdTimer.stop();
		}

		~PhotoTimer()
		{
			clear();
		};

		PhotoTimer(shared_ptr<PhotoboothSettings> settings)
		{		
			reset(settings);
		}

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings =  _settings;
			fon		 =  settings->getTextures()["fon3"]->get();
			font	 =  settings->getFonts()["helvetica40"]->get();
			font100	 =  settings->getFonts()["helvetica100"]->get();
		}

		void start()
		{
			seconds = settings->getData().seconds;
			cdTimer.start();
			console()<<"start PhotoTimer"<<endl;			
		}		

		void update() override
		{
			seconds = (settings->getData().seconds - (int)cdTimer.getSeconds());

			if(seconds < 0)
			{
				clear();
				nextLocationSignal();
			}

			cameraCanon().update();
		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			cameraCanon().draw();
			textTools().textFieldDraw("ÒÀÉÌÅÐ", &font, Vec2f(100.0f, 100.0f), Color::white());
			textTools().textFieldDraw(to_string(seconds), &font100, Vec2f(200.0f, 200.0f), Color::white());
		}

		void mouseUpHandler(Vec2i vec)
		{
			clear();
			nextLocationSignal();
		}
	};
}