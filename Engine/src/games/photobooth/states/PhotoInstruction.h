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
	class PhotoInstruction:public IPhotoboothLocation
	{
		Texture fon;
		ci::Font font;

	public:	

		PhotoInstruction(shared_ptr<PhotoboothSettings> settings)
		{
			reset(settings);		
		};

		~PhotoInstruction(){};

		void reset(shared_ptr<PhotoboothSettings> _settings) override
		{
			settings = _settings;
			fon = settings->getTextures()["fon1"]->get();
			font =  settings->getFonts()["helvetica40"]->get();
		}

		void start()
		{
			console()<<"start Instruction"<<endl;
		}

		void update() override
		{

		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			gl::color(Color::white());
			textTools().textFieldDraw("ÈÍÑÒÐÓÊÖÈß", &font, Vec2f(100, 100), Color::white());
		}

		void mouseUpHandler( Vec2i vec)
		{
			nextLocationSignal();
		}
	};
}