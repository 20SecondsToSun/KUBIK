#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ILocation.h"

using namespace std;
using namespace ci::signals;
using namespace ci::app;

class PhotoFilter:public ILocation
{

public:	
	~PhotoFilter()
	{
		console()<<"photo filter destructor"<<endl;
	};


	void init()
	{

	}

	void reset()
	{

	}

	void draw()
	{
		gl::color(Color(0, 0, 1));
		gl::drawSolidRect(getWindowBounds());
		gl::color(Color::white());
	}
	
	void mouseUpHandler( Vec2i vec)
	{
		//console()<<" mouse up :::: PhotoInstruction "<<endl;
		nextLocationSignal();
	}
};