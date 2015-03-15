#pragma once
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			class PhotoInstruction:public IPhotoboothLocation
			{
				ci::gl::Texture fonTex, titleTex;		
				ci::Vec2f titleTexPos;

				ci::Anim<float> alphaAnim;
				float animTime;
				//float titlePositionY;
				void hideAnimation(EventGUIRef& event);				
				
			public:	
				PhotoInstruction(PhotoboothSettingsRef settings);
				virtual void reset(PhotoboothSettingsRef set);
				virtual void start();
				virtual void stop();
				void update();
				void draw();
			};
			typedef shared_ptr<PhotoInstruction> PhotoInstructionRef;
		}
	}
}