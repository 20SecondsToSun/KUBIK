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
		
	}

	void draw()
	{	
		gl::color(Color::white());
		gl::drawSolidRect(getWindowBounds());
		console()<<msg.getMessage()<<endl;
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
};