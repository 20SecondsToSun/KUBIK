#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ILocation.h"
#include "PhotoboothSettings.h"

using namespace std;
namespace kubik
{
	class IPhotoboothLocation 
	{

	protected:
		shared_ptr<PhotoboothSettings> settings;
	public:	
		~IPhotoboothLocation(){};
		virtual void draw() = 0;
		virtual void start() = 0;
		virtual void reset(shared_ptr<PhotoboothSettings> config) = 0;	
	
		signal<void(void)> nextLocationSignal;

		virtual void mouseUpHandler( Vec2i vec)
		{

		}
	};
}