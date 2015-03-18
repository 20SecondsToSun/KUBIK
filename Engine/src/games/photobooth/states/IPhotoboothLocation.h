#pragma once
#include "PhotoboothSettings.h"
#include "model/PhotoStorage.h"
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
				ci::Anim<float> titleAlpha, titleScale, titleFilterAlpha;
				ci::Anim<ci::Vec2f> titleAnimPosition;
				ci::gl::Texture title, screenshot;
				void setLastScreenShot();				

			public:	
				static const int NEXT_LOC = 0;	

				IPhotoboothLocation();

				virtual void update() = 0;
				virtual void start() = 0;
				virtual void stop(){};
				virtual void reset(PhotoboothSettingsRef config);	
				virtual void stopAllTweens();

				void nextLocationSignal(EventGUIRef& event = EventGUIRef(new EventGUI()));				
				void hideAnimationComplete();				
				void fillBg();
				void drawTitle();
			};

			typedef shared_ptr<IPhotoboothLocation> IPhotoboothLocationRef;
		}
	}
}