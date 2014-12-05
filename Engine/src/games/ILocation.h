#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci::signals;

class ILocation
{

public:	
	~ILocation(){};
	virtual void draw() = 0;
	virtual void init() = 0;
	virtual void reset() = 0;
//	virtual void addMouseUpListener() = 0;
//	virtual void removeMouseUpListener() = 0;
	signal<void(void)> nextLocationSignal;

	virtual void mouseUpHandler( Vec2i vec)
	{
		
	}
};