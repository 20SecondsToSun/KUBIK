#pragma once

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
		int		currentShot, shotsNum;
		int		secBetweenShots;
		bool	checkTimerInUpdate;
		Texture fon;
		Font	font;		
		Timer	shTimer;	
		PhotoStorageRef photoStorage;

	public:
		PhotoShooting(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage):photoStorage(photoStorage)
		{	
			reset(settings);		
		};

		~PhotoShooting()
		{

		};

		void start() override
		{
			console()<<"start PhotoShooting"<<endl;
			currentShot = 1;
			shotsNum = settings->getPhotoShots();
			secBetweenShots = 0;//settings->getData().secondsBetweenShots;

			photoStorage->prepare(shotsNum);
			///
			for (int i = 1; i <= shotsNum; i++)
			{
				photoStorage->setPhotoPath(i, getAppPath().string() + to_string(i) + ".JPG");
			}
			nextLocationSignal();
			///
			connect_once(cameraCanon().photoDownloadedEvent, bind(&PhotoShooting::photoDownloadHandler, this, placeholders::_1));
			shooting();
		}

		void photoDownloadHandler(const string& downloadedPath) 
		{
			photoStorage->setPhotoPath(currentShot, downloadedPath);

			if(++currentShot > shotsNum)		
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
			else if(shTimer.getSeconds() > secBetweenShots)
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
			photoStorage->setScreenshot(currentShot, copyWindowSurface());
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
			textTools().textFieldDraw("ÔÎÒÎÃÐÀÔÈÐÓÅÌ", &font, Color::white(), Vec2f(100.0f, 100.0f));
			textTools().textFieldDraw("ÊÎËÈ×ÅÑÒÂÎ ÑÍÈÌÊÎÂ " + to_string(shotsNum), &font, Color::white(), Vec2f(100.0f, 200.0f));
			textTools().textFieldDraw("ÂÐÅÌß ÎÄÍÎÃÎ ÑÍÈÌÊÀ(ÑÅÊ.) " + to_string(secBetweenShots), &font, Color::white(), Vec2f(100.0f, 300.0f));
		}

		void reset(PhotoboothSettingsRef _settings) override
		{
			settings = _settings;
			fon  = settings->getTexture("fon1");
			font = settings->getFont("helvetica40");
		}

		void mouseUpHandler(Vec2i vec) override
		{
			shTimer.stop();
			cameraCanon().photoDownloadedEvent.disconnect_all_slots();
			nextLocationSignal();
		}
	};
	typedef shared_ptr<PhotoShooting>	 PhotoShootingRef;
}