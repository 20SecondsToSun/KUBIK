#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci::signals;

namespace kubik
{
	class ILocation
	{

	public:	
		~ILocation(){};

		virtual void draw() = 0;
		virtual void init(shared_ptr<ISettings>) = 0;
		virtual void reset(shared_ptr<ISettings>) = 0;
	
		signal<void(void)> nextLocationSignal;

		virtual void mouseUpHandler( Vec2i vec)
		{

		}
	};
}