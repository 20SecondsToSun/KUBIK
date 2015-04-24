#pragma once
#include "Checker.h"
#include "HashCheckerEvent.h"
#include "gui/SimpleSpriteButton.h"
#include "gui/ImageButtonSprite.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class KeyBackground> KeyBackgroundRef;

		class KeyBackground : public Sprite
		{			
			ci::Anim<float> alphaColorPlashka, alphaColorBg, animPositionY;
			void mouseUpHandler(EventGUIRef& event);
			SimpleSpriteButtonRef btn;			
			ci::Vec2f initPosition;
			bool showing;
			float plashkaHeight, bgHeight;
			ci::gl::Texture closeKeyboard;

		public:
			static const int HIDE_KEYBOARD = 0;

			KeyBackground(const Vec2f& initPosition = Vec2f(-166.0f, 918.0f));
			KeyBackground(const Vec2f& initPosition, const ci::gl::Texture& closeKeyboard);

			void show(const EaseFn& eFunc, float time);
			void hide(const EaseFn& eFunc, float time);
			void close();

			void drawLayout() override;
		};
	}
}