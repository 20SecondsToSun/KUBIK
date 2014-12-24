#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "CameraAdapter.h"
#include "model/PhotoStorage.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class PhotoProcessing: public IPhotoboothLocation
	{
		int	 currentShot;
		bool checkTimerInUpdate;
	
		Texture fon;
		Font	font;		
		Timer	shTimer;	
		PhotoStorageRef photoStorage;		

	public:
		PhotoProcessing(PhotoboothSettingsRef settings, PhotoStorageRef _photoStorage)
		{
			photoStorage = _photoStorage;
			reset(settings);		
		};

		~PhotoProcessing()
		{

		};

		void start() override
		{
			console()<<"start PhotoProcessing"<<endl;
			photoStorage->loadHiRes();
		}	

		void update() override
		{	
			if(photoStorage->isHiResLoaded())
			{
				photoStorage->prepareAllResizes();
				nextLocationSignal();
			}
		}		

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
		
			gl::color(Color::white());
			textTools().textFieldDraw("ÂÛÃÐÓÆÀÞ ÔÎÒÎ", &font, Vec2f(100.0f, 100.0f), Color::white());
		}

		void reset(PhotoboothSettingsRef _settings) override
		{
			settings = _settings;
			fon  = settings->getTexture("fon1");
			font = settings->getFont("helvetica40");
		}

		void mouseUpHandler(Vec2i vec) override
		{
			nextLocationSignal();
		}
	};

	typedef	shared_ptr<PhotoProcessing> PhotoProcessingRef;
}