#pragma once
#include "Utils.h"
#include "PhotoboothSettings.h"
#include "gui/Sprite.h"
#include "gui/EventGUI.h"
#include "gui/ImageButtonSprite.h"
#include "VirtualKeyboard.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class Popup> PopupRef;

			class Popup: public Sprite
			{
			protected:
				ci::Anim<float> alphaAnim;
				ci::ColorA bgColor, headColor;
				ci::gl::Texture closeIcon;			
				kubik::ImageButtonSpriteRef closeBtn;

				virtual void showAnimComplete();
				virtual void hideAnimComplete();
				virtual void initVirtualKeyboard();

				void connectCloseBtn();
				void disconnectCloseBtn();

			public:
				static const int POPUP_CLOSED = 1;
				ci::Font inputFont;

				Popup(kubik::config::PhotoboothSettingsRef settings);

				virtual void kill();
				virtual void show();
				virtual void hide(EventGUIRef& event);
				virtual void draw();
				virtual void drawBackgrounds();
				virtual void close();
			};
		}
	}
}