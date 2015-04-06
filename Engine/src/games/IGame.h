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
			DRAW
		}
		state;

		ci::gl::Texture	screenshot;

	public:	
		static const int ENABLE_GAME_CLOSE = 1;
		static const int DISABLE_GAME_CLOSE = 2;
		static const int SHOW_CONTROLS = 3;
		static const int HIDE_CONTROLS = 4;		

		virtual ~IGame(){};
		virtual void draw(){};
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void clean(){};

		void initShowAnimation()
		{
			screenshot = getScreenShot();
			state = SHOW_ANIM;
			timeline().apply(&animX, 1080.0f, 0.0f, 0.9f, EaseOutCubic()).finishFn(bind(&IGame::showAnimationComplete, this));
			timeline().apply(&animX1, 0.0f, -500.0f, 0.9f, EaseOutCubic());
			timeline().apply(&alpha, 1.0f, 0.2f, 0.9f, EaseOutCubic());
		}

		virtual void showAnimationComplete() = 0;

		virtual void screenshotDraw()
		{
			gl::pushMatrices();
			gl::translate(animX1, 0.0f);
			gl::color(ColorA(1.0f, 1.0f, 1.0f, alpha));
			if (screenshot)
				gl::draw(screenshot);
			gl::color(Color::white());
			gl::popMatrices();
		}		
	};
}