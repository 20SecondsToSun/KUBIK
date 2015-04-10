#pragma once
#include "Checker.h"
#include "HashCheckerEvent.h"
#include "gui/SimpleSpriteButton.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class KeyBackground> KeyBackgroundRef;

		class KeyBackground : public Sprite
		{			
			ci::Anim<float> alphaColor;
			void mouseUpHandler(EventGUIRef& event);
			SimpleSpriteButtonRef btn;
			bool showing;
			Vec2f initPosition;
		public:
			static const int HIDE_KEYBOARD = 0;

			KeyBackground(const Vec2f& initPosition = Vec2f(-166.0f, 918.0f));

			void show(const EaseFn& eFunc, float time);
			void hide(const EaseFn& eFunc, float time);
			void close();

			virtual void drawLayout() override;
		};
	}
}