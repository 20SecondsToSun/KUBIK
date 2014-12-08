#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "IScreen.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class ServicePopup:public IScreen
{
public:	
	ServicePopup():IScreen()
	{	
		font = Font(loadFile( getAppPath().string() + "data\\fonts\\Helvetica Neue.ttf"), 30);
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

	void init()
	{
		font = designTexures["helvetica30"]->font;
	}

	void setMessage(const char* msg)
	{			
		this->msg = string(msg);
	}

protected:
	void setTextures()
	{	
		addToDictionary("helvetica30",  getAppPath().string() + "data\\fonts\\Helvetica Neue.ttf", resourceType::FONT, loadingType::FULL_PATH, 30);
	}

private:	
	string msg;
	Font font;
};