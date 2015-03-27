#pragma once

#include "ConfigSettings.h"
#include "StartNewActivityEvent.h"
#include "CloseActivityEvent.h"
#include "VirtualKeyboard.h"
#include "gui/SimpleSpriteButton.h"
#include "gui/Sprite.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class NewActivityPopup> NewActivityPopupRef;

		class NewActivityPopup: public Sprite
		{
		public:
			static const int OPENED = 1;
			static const int HIDE_EVENT = 2;
			static const int START_NEW_COMPAIN = 3;			
			static const int HIDED = 4;

			NewActivityPopup(ConfigSettingsRef configSett);
			virtual void draw();
			void drawBackgrounds();
			void drawTitles();
			void drawInputFieldBackground();
			void drawCloseBlock();
			virtual void setAlpha(float alpha);

			void show(const ci::EaseFn& eFunc = EaseOutCubic(), float time = 0.4f);
			void hide(const ci::EaseFn& eFunc = EaseOutCubic(), float time = 0.3f);

			std::string getCompainName();

			void alphAnimationUpdate();
			void showAnimationFinish();
			void initVirtualKeyboard();
			void inputTouchHandler();
			void closeHandler(EventGUIRef& event);
			void newCompainHandler(EventGUIRef& event);
			void disconnect();
			void hideAnimationFinish();
			bool emptyInputField();
			
		protected:
			ConfigSettingsRef configSett;
			ci::ColorA bgColor, headColor, titlesColor, inputFieldColor, btnStartColor;
			ci::Anim<float> alpha, redFocusAlpha;
			ci::gl::Texture subTitle, mainTitle, title, begin, closeIcon, closeTitle, inputField, redFocus;

			SimpleSpriteButtonRef closeBtn, startBtn;
			ci::Vec2f closeBtnPos, inputFieldPos;

			float yPositionInputField;

			void showRedFocusStroke();
		};
	}
}
