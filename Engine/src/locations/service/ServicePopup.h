#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "IDrawable.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class ServicePopup:public IDrawable
{
public:
	ServicePopup()
	{
		font =  Font(loadFile(getAssetPath("fonts/Helvetica Neue.ttf")), 30);	
	}

	void draw()
	{	
		gl::color(Color::white());
		gl::drawSolidRect(getWindowBounds());

		Texture textTex = textTools().getTextField(msg.getMessage(), &font, ColorA(1,0,0,1));
		gl::pushMatrices();			
			float shiftX = (getWindowWidth() - textTex.getWidth()) * 0.5;
			float shiftY = (getWindowHeight() - textTex.getHeight()) * 0.5;		
			gl::translate(shiftX, shiftY);
			gl::draw(textTex);
		gl::popMatrices();
	}	

	void init()
	{

	}

	void setMessage(ServiceMessage _msg)
	{
		
		msg = _msg;
	}

private:	
	ServiceMessage msg;
	Font font;
};