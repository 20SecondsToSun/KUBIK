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
		addToDictionary("helvetica30",  getAppPath().string() + "data\\fonts\\Helvetica Neue.ttf", resourceType::FONT, loadingType::FULL_PATH, 30);
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
		font = designTexures["helvetica30"]->font;
	}

	void setMessage(ServiceMessage _msg)
	{		
		msg = _msg;
	}

private:	
	ServiceMessage msg;
	Font font;
};