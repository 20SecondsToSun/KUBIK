#pragma once

#include "PhotoboothSettings.h"
#include "model/PhotoStorage.h"
#include "gui/Sprite.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef std::shared_ptr<class IPhotoboothLocation> IPhotoboothLocationRef;

			class IPhotoboothLocation : public Sprite
			{
			protected:
				static config::PhotoboothSettingsRef settings;
				ci::gl::TextureRef bckgrnd;
				float titlePositionY, animShowTitleTime;

				ci::Anim<float> titleAlpha, titleScale, titleFilterAlpha;
				ci::Anim<ci::Vec2f> titleAnimPosition;
				ci::gl::Texture title, screenshot;
				void setLastScreenShot();				

			public:	
				static const int NEXT_LOC			= 100;
				static const int BEGIN_ANIM			= 101;
				static const int COMPLETE_ANIM		= 102;	
				static const int ENABLE_GAME_CLOSE  = 103;
				static const int DISABLE_GAME_CLOSE = 104;
				static const int CLOSE_LOCATION		= 105;

				IPhotoboothLocation();

				virtual void update() = 0;
				virtual void start()  = 0;
				virtual void stop(){};
				virtual void reset(config::PhotoboothSettingsRef config);
				virtual void stopAllTweens();

				void nextLocationSignal(EventGUIRef& event = EventGUIRef(new EventGUI()));				
				void hideAnimationComplete();				
				void fillBg();
				void drawTitle();
			};		
		}
	}
}
