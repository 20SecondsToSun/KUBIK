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

		public:
			static const int HIDE_KEYBOARD = 0;

			KeyBackground();

			void show(const EaseFn& eFunc, float time);
			void hide(const EaseFn& eFunc, float time);
			void close();

			virtual void drawLayout() override;
		};
	}
}