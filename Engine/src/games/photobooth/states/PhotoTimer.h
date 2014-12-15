#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ILocation.h"
#include "IPhotoboothLocation.h"

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
		bool isRunning;
		ci::signals::connection updSignal;

	public:

		void clear()
		{
			updSignal.disconnect();
			isRunning = false;
			cdTimer.stop();
		}

		~PhotoTimer()
		{
			clear();
		};

		PhotoTimer(shared_ptr<PhotoboothSettings> settings)
		{
			isRunning = false;
			reset(settings);
		}

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings = _settings;
			fon		 = settings->getTextures()["fon3"]->get();
			font	 =  settings->getFonts()["helvetica40"]->get();
			font100	 =  settings->getFonts()["helvetica100"]->get();
		}

		void start()
		{
			seconds = settings->getData().seconds;
			cdTimer.start();
			console()<<"start PhotoTimer"<<endl;

			if(!isRunning)
			{
				updSignal = App::get()->getSignalUpdate().connect(bind(&PhotoTimer::update, this));	
				isRunning = true;
			}			
		}		

		void update()
		{
			seconds = (settings->getData().seconds - (int)cdTimer.getSeconds());
			if(seconds < 0)
			{
				clear();
				nextLocationSignal();
			}
		}

		void draw()
		{
			gl::draw(fon, getWindowBounds());
			textTools().textFieldDraw("ÒÀÉÌÅÐ", &font, Vec2f(100.0f, 100.0f), Color::white());
			textTools().textFieldDraw( to_string(seconds), &font100, Vec2f(200.0f, 200.0f), Color::white());
		}

		void mouseUpHandler( Vec2i vec)
		{
			console()<<"mouseUP PhotoTimer"<<endl;
			clear();
			nextLocationSignal();
		}
	};
}