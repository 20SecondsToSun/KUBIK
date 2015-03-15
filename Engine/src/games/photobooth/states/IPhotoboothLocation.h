#pragma once

#include "PhotoboothSettings.h"
#include "gui/Sprite.h"

using namespace std;
using namespace kubik::config;

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			class IPhotoboothLocation : public Sprite
			{	
			protected:
				static PhotoboothSettingsRef settings;
				static ci::gl::Texture bckgrnd;
				float titlePositionY;

				float animShowTitleTime;
				ci::Anim<float> titleAlpha;
				ci::Anim<ci::Vec2f> titleAnimPosition;

			public:	
				static const int NEXT_LOC = 0;	

				IPhotoboothLocation();

				virtual void update() = 0;
				virtual void start() = 0;
				virtual void stop(){};
				virtual void reset(PhotoboothSettingsRef config);	

				void nextLocationSignal(EventGUIRef& event = EventGUIRef(new EventGUI()));				
				void hideAnimationComplete();				
				void fillBg();
			};

			typedef shared_ptr<IPhotoboothLocation> IPhotoboothLocationRef;
		}
	}
}