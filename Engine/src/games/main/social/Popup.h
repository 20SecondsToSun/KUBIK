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
		typedef	std::shared_ptr<class Popup> PopupRef;

		class Popup: public Sprite
		{
			typedef boost::signals2::signal<void(SharingType, const std::string& data)> ShareCompleteSignal;

		protected:
			ci::Anim<float> alphaAnim;
			ci::ColorA bgColor, headColor;
			ci::gl::Texture closeIcon;
			static ci::gl::Texture backgroundImage;

			kubik::ImageButtonSpriteRef closeBtn;

			virtual void showAnimComplete();
			virtual void hideAnimComplete();
			virtual void initVirtualKeyboard();

			void connectCloseBtn();
			void disconnectCloseBtn();
			void connectAllFieldAsButton();
			void disconnectAllFieldAsButton();

		public:
			static const int POPUP_CLOSED = 1;
			ci::Font inputFont;				
			ShareCompleteSignal shareCompleteSignal;
			Popup(kubik::config::ISettingsRef settings);

			virtual void kill();
			virtual void show();
			virtual void hide(EventGUIRef& event);
			virtual void hideQuick(EventGUIRef& event);				
			virtual void draw();						
			virtual void close();
			virtual void drawBackgrounds();
			void setBackground(const ci::gl::Texture& texture);
		};		
	}
}