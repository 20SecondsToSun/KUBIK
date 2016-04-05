#pragma once

#include "IScreen.h"
#include "ISettings.h"
#include "Types.h"
#include "gui/Sprite.h"

using namespace std;
using namespace ci::signals;

namespace kubik
{
	class IGame:public IScreen, public Sprite
	{
	protected:
		ci::Anim<float> animX, animX1, alpha;

		enum locationStates
		{
			SHOW_ANIM,
			DRAW,
			CAMERA_DISCONNECT
		}
		state;

		ci::gl::Texture	screenshot;		

	public:	
		static const int ENABLE_GAME_CLOSE  = 1;
		static const int DISABLE_GAME_CLOSE = 2;
		static const int SHOW_CONTROLS		= 3;
		static const int HIDE_CONTROLS		= 4;
		static const int CLOSE_LOCATION		= 5;

		virtual ~IGame();
		virtual void draw();
		virtual void start()  = 0;
		virtual void update() = 0;
		virtual void clean();

		void initShowAnimation();

		virtual void showAnimationComplete() = 0;
		virtual void screenshotDraw();
	};
}