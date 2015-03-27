#pragma once
#include "GameSettingsSprite.h"
#include "InstakubSettings.h"
#include "SearchBlock.h"
#include "HashTagBlock.h"
#include "InstaPhotoCardStyle.h"
#include "VirtualKeyboard.h"
#include "KeyBackground.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class InstaErrorPopup> InstaErrorPopupRef;

		class InstaErrorPopup : public Sprite
		{			
			ci::Anim<float> alphaColor;
			void mouseUpHandler(EventGUIRef& event);
			SimpleSpriteButtonRef btn;
			bool showing;
			gl::Texture image;

		public:
			static const int HIDE_POPUP = 0;

			InstaErrorPopup(const gl::Texture& image);

			void show(const EaseFn& eFunc, float time);
			void hide(const EaseFn& eFunc, float time);
			void close();

			virtual void drawLayout() override;
		};
	}
}