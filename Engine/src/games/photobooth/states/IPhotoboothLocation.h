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
		static PhotoboothSettingsRef settings;

	public:	
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void start() = 0;
		virtual void reset(PhotoboothSettingsRef config) = 0;	
		virtual void mouseUpHandler(ci::Vec2i vec) = 0;

		signal<void(void)> nextLocationSignal;
	};

	typedef shared_ptr<IPhotoboothLocation> IPhotoboothLocationRef;
}