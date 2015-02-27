#pragma once
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"

namespace kubik
{
	namespace games
	{
		class PhotoInstruction:public IPhotoboothLocation
		{
			ci::gl::Texture fonTex, titleTex;		
			ci::Vec2f titleTexPos;

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