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
	class PhotoShooting: public IPhotoboothLocation
	{
		int		currentShot;
		bool	checkTimerInUpdate;
		Texture fon;
		Font	font;		
		Timer	shTimer;	
		shared_ptr<PhotoStorage> photoStorage;

	public:
		PhotoShooting(shared_ptr<PhotoboothSettings> settings, shared_ptr<PhotoStorage> _photoStorage)
		{
			photoStorage = _photoStorage;
			reset(settings);		
		};

		~PhotoShooting()
		{

		};

		void start() override
		{
			console()<<"start PhotoShooting"<<endl;
			currentShot = 0;
			photoStorage->clear();

			connect_once(cameraCanon().photoDownloadedEvent, bind(&PhotoShooting::photoDownloadHandler, this, placeholders::_1));
			shooting();
		}

		void photoDownloadHandler(const string& downloadedPath) 
		{
			photoStorage->setPhotoPath(downloadedPath);

			if(++currentShot >= settings->getPhotoShots())		
			{
				cameraCanon().photoDownloadedEvent.disconnect_all_slots();
				nextLocationSignal();
			}
			else if(isTimerComplete())	
			{
				shooting();		
			}
			else			
				checkTimerInUpdate = true;			
		}

		bool isTimerComplete()
		{
			if (shTimer.isStopped())
			{
				return true;
			}
			else if(shTimer.getSeconds() > settings->getData().secondsBetweenShots)
			{
				shTimer.stop();
				return true;
			}

			return false;
		}

		void shooting()
		{
			checkTimerInUpdate = false;
			shTimer.start();
			photoStorage->addScreenshot(copyWindowSurface());
			cameraCanon().takePicture();
		}		

		void update() override
		{
			cameraCanon().update();

			if(checkTimerInUpdate && isTimerComplete())
				shooting();			
		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			cameraCanon().draw();

			gl::color(Color::white());
			textTools().textFieldDraw("ÔÎÒÎÃÐÀÔÈÐÓÅÌ", &font, Vec2f(100.0f, 100.0f), Color::white());
			textTools().textFieldDraw("ÊÎËÈ×ÅÑÒÂÎ ÑÍÈÌÊÎÂ " + to_string(settings->getPhotoShots()), &font, Vec2f(100.0f, 200.0f), Color::white());
			textTools().textFieldDraw("ÂÐÅÌß ÎÄÍÎÃÎ ÑÍÈÌÊÀ(ÑÅÊ.) " + to_string(settings->getData().secondsBetweenShots), &font, Vec2f(100.0f, 300.0f), Color::white());
		}

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings = _settings;
			fon  = settings->getTextures()["fon1"]->get();
			font =  settings->getFonts()["helvetica40"]->get();
		}

		void mouseUpHandler(Vec2i vec) override
		{
			shTimer.stop();
			cameraCanon().photoDownloadedEvent.disconnect_all_slots();
			nextLocationSignal();
		}
	};
}