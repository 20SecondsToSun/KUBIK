#pragma once
#include "gui/Sprite.h"

namespace kubik
{
	namespace config
	{
		typedef shared_ptr<class GameSettingsSprite> GameSettingsSpriteRef;

		class GameSettingsSprite: public Sprite
		{
		public:
			virtual void showAnimate(ci::EaseFn eFunc, float time) = 0;
			virtual void hideAnimate(ci::EaseFn eFunc, float time) = 0;
		};
	}
}