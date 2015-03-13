#pragma once

#include "PhotoboothSettings.h"
#include "gui/Sprite.h"

using namespace std;
using namespace kubik::config;

namespace kubik
{
	class IPhotoboothLocation : public Sprite
	{	
	protected:
		static PhotoboothSettingsRef settings;
		static ci::gl::Texture bckgrnd;

	public:	
		static const int NEXT_LOC = 0;	

		virtual void update() = 0;
		virtual void start() = 0;
		virtual void stop(){};
		virtual void reset(PhotoboothSettingsRef config);	
	
		void nextLocationSignal(EventGUIRef& event = EventGUIRef(new EventGUI()));
		void fillBg();
	};

	typedef shared_ptr<IPhotoboothLocation> IPhotoboothLocationRef;
}