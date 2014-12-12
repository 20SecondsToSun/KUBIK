#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "IScreen.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

namespace kubik
{
	class ServicePopup:public IScreen
	{
	public:	
		ServicePopup():IScreen()
		{	
			string path = getAppPath().string() + "data\\fonts\\Helvetica Neue.ttf";
			DataSourceRef ref = loadFile(path);
			font = Font(ref, 30);
			
			//setTextures();
		}

		void draw()
		{	
			gl::color(Color::white());
			gl::drawSolidRect(getWindowBounds());

			Texture textTex = textTools().getTextField(msg, &font, ColorA(1,0,0,1));
			gl::pushMatrices();			
			float shiftX = (getWindowWidth() - textTex.getWidth()) * 0.5;
			float shiftY = (getWindowHeight() - textTex.getHeight()) * 0.5;		
			gl::translate(shiftX, shiftY);
			gl::draw(textTex);
			gl::popMatrices();	
		}	

		void init(shared_ptr<ISettings> config) override
		{

		}

		void reset(shared_ptr<ISettings> config) override{};

		void setMessage(const char* msg)
		{			
			this->msg = string(msg);
		}

	private:	
		string msg;
		Font font;
	};
}