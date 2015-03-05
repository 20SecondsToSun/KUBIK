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
				
		Timer	shTimer;	
		PhotoStorageRef photoStorage;

		Texture countsTex, seekTex;				
		ci::Vec2f countsTexPos, seekTexPos, seekTexPos0;
		float progressBlockStartY;

	public:
		PhotoShooting(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage)
			:photoStorage(photoStorage),
			 progressBlockStartY(0.0f)
		{	
			reset(settings);		
		};

		~PhotoShooting()
		{

		};

		void start() override
		{
			console()<<"start PhotoShooting"<<endl;
			currentShot = 5;
			return;
			//shotsNum = settings->getPhotoShots();
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
			//cameraCanon().update();

			//if(checkTimerInUpdate && isTimerComplete())
			//	shooting();			
		}

		void draw() override
		{
			fillBg();
		
			//cameraCanon().draw();

			//gl::color(Color::white());
			//textTools().textFieldDraw("ÔÎÒÎÃÐÀÔÈÐÓÅÌ", &font, Color::white(), Vec2f(100.0f, 100.0f));
			//textTools().textFieldDraw("ÊÎËÈ×ÅÑÒÂÎ ÑÍÈÌÊÎÂ " + to_string(shotsNum), &font, Color::white(), Vec2f(100.0f, 200.0f));
			//textTools().textFieldDraw("ÂÐÅÌß ÎÄÍÎÃÎ ÑÍÈÌÊÀ(ÑÅÊ.) " + to_string(secBetweenShots), &font, Color::white(), Vec2f(100.0f, 300.0f));

			drawProgressBlock();
			drawDashedFrame();
		}

		void drawProgressBlock()
		{
			gl::pushMatrices();
			gl::translate(0.0f, progressBlockStartY);
			gl::color(Color::hex(0xD60e0d0a));
			gl::drawSolidRect(Rectf(0.0f, 0.0f, getWindowWidth(), 232.0f));
			gl::color(Color::white());
			gl::translate(0.0f, 69.0f);
			seekTexPos = seekTexPos0 + Vec2f((currentShot - 1) * 197.0f, 0.0f);
			gl::draw(seekTex, seekTexPos);
			gl::draw(countsTex, countsTexPos);
			gl::popMatrices();

		}

		void drawDashedFrame()
		{	
			gl::color(Color::hex(0xbcbdbd));
			float lineWidth = 4, space = 8;
			float frameHeight = 980;
			
			drawtool().drawDashedLine(50, 252, getWindowWidth() - 50, 252, lineWidth, space);
			drawtool().drawDashedLine(50, 252 + frameHeight, getWindowWidth() - 50, 252 + frameHeight, lineWidth, space);
			drawtool().drawDashedLine(50, 252, 50, 252 + frameHeight, lineWidth, space);
			drawtool().drawDashedLine(getWindowWidth() - 50, 252, getWindowWidth() - 50, 252 + frameHeight, lineWidth, space);
			gl::color(Color::white());
		}

		void reset(PhotoboothSettingsRef _settings) override
		{
			settings = _settings;
			countsTex   = settings->getTexture("counts");
			seekTex		=  settings->getTexture("seek");
			countsTexPos= Vec2f(0.5 * (getWindowWidth() - countsTex.getWidth()), 0);
			seekTexPos0 = Vec2f(146.0f - seekTex.getWidth()*0.5f, 0.5f*(countsTex.getHeight() - seekTex.getHeight()));

		}
	};
	typedef shared_ptr<PhotoShooting>	 PhotoShootingRef;
}