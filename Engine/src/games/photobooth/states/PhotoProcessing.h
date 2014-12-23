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
		int		currentShot;
		bool	checkTimerInUpdate;
		Texture fon;
		Font	font;		
		Timer	shTimer;	
		shared_ptr<PhotoStorage> photoStorage;
		boost::shared_ptr<boost::thread> loadingThread;
		bool threadIsRunning;

	public:
		PhotoProcessing(shared_ptr<PhotoboothSettings> settings, shared_ptr<PhotoStorage> _photoStorage)
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
			threadIsRunning = true;
			loadingThread = boost::shared_ptr<boost::thread>(new boost::thread(&PhotoProcessing::loadPhotos, this));
		}

		void loadPhotos() 
		{
			for (auto path : photoStorage->getPhotoPaths())
			{
				Surface image = Surface(loadImage(loadFile(path)));
				photoStorage->setHiResPhoto(image);				
			}
			threadIsRunning = false;
		}


		void update() override
		{	
			if(!threadIsRunning)
				nextLocationSignal();
		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
		
			gl::color(Color::white());
			textTools().textFieldDraw("¬€√–”∆¿ﬁ ‘Œ“Œ", &font, Vec2f(100.0f, 100.0f), Color::white());
		}

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings = _settings;
			fon  = settings->getTextures()["fon1"]->get();
			font =  settings->getFonts()["helvetica40"]->get();
		}

		void mouseUpHandler(Vec2i vec) override
		{
			nextLocationSignal();
		}
	};
}