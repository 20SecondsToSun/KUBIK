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

		PhotoTimer(PhotoboothSettingsRef settings)
		{		
			reset(settings);
		}

		void reset(PhotoboothSettingsRef _settings) override
		{
			settings =  _settings;
			fon		 =  settings->getTexture("fon3");
			font	 =  settings->getFont("helvetica40");
			font100	 =  settings->getFont("helvetica100");
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

			/*Surface surf = cameraCanon().getLiveSurface();
			ImageFilter::BlackWhiteFilter filter;
			surf = filter.process(surf);
			gl::draw(surf);*/

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

	typedef shared_ptr<PhotoTimer> PhotoTimerRef;
}