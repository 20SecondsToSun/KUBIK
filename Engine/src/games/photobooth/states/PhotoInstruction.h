#pragma once

#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"

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

		PhotoInstruction(PhotoboothSettingsRef settings)
		{
			reset(settings);		
		};

		~PhotoInstruction(){};

		void reset(PhotoboothSettingsRef _settings) override
		{
			settings = _settings;
			fon = settings->getTexture("fon1");
			font =  settings->getFont("helvetica40");
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

		void mouseUpHandler(Vec2i vec)
		{
			nextLocationSignal();
		}
	};
	typedef shared_ptr<PhotoInstruction> PhotoInstructionRef;
}