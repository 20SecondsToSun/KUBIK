#pragma once

#include "PhotoboothSettings.h"

using namespace std;
using namespace kubik::config;

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

		SignalVoid nextLocationSignal;
	};

	typedef shared_ptr<IPhotoboothLocation> IPhotoboothLocationRef;
}