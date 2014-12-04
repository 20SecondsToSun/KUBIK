#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ILocation.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;

class PhotoInstruction:public ILocation
{

public:	
	~PhotoInstruction(){};

	void init()
	{

	}

	void draw()
	{
		gl::color(Color(0, 1, 0));
		gl::drawSolidRect(getWindowBounds());
		gl::color(Color::white());
	}

	void mouseUpHandler( Vec2i vec)
	{
		console()<<" mouse up :::: PhotoInstruction "<<endl;
		nextLocationSignal();
	}
};