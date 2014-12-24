#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "cinder/gl/Texture.h"
#include "model/PhotoStorage.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class PhotoTemplate:public IPhotoboothLocation
	{
		Texture fon, stickerTex;
		Font font;
		PhotoStorageRef photoStorage;
		bool isPrint;

	public:
		PhotoTemplate(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage):photoStorage(photoStorage)
		{		
			reset(settings);		
		};

		~PhotoTemplate()
		{

		};

		void start() override
		{
			console()<<"start PhotoTemplate"<<endl;
			photoStorage->createSharingPhotos(stickerTex);	

			if (isPrint)
				photoStorage->createPrintTemplatePhoto(stickerTex);

			if(settings->getData().photoNum < 3 || !isPrint)
				nextLocationSignal();
		}

		void reset(PhotoboothSettingsRef _settings) override
		{
			settings	= _settings;
			fon			= settings->getTexture("fon1");
			font		= settings->getFont("helvetica40");
			stickerTex  = settings->getActiveStickerTex();	
			isPrint		= settings->getData().sharing.isPrint;
		}

		void update() override
		{

		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			gl::color(Color::white());
			textTools().textFieldDraw("бшаепхре ьюакнм", &font, Vec2f(100.0f, 100.0f), Color::white());

			if(stickerTex)
				gl::draw(stickerTex, Vec2f(100.0f, 300.0f));
		}

		void mouseUpHandler(Vec2i vec) override
		{
			nextLocationSignal();
		}
	};

	typedef	shared_ptr<PhotoTemplate> PhotoTemplateRef;
}