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
				ci::Anim<float> alphaAnim, alphaError;
				ci::ColorA bgColor, headColor, addEmailFontColor;
				ci::gl::Texture closeIcon, addEmailIcon, borderIcon, borderIconRed;
				ci::Vec2f borderIconPos, initAddEmailPosition,	shiftEmailPosition;

				kubik::ImageButtonSpriteRef closeBtn, addEmailBtn;

				void showAnimComplete();
				void hideAnimComplete();

				void drawInputField();
				void initVirtualKeyboard();
				void drawAddedEmails();
				void clearEmails();
				bool handleInputField();
				void showRedFocusStroke();
				void addEmailToList();
				std::vector<std::string> emails;
				bool showAddEmail;

				struct EmailTex
				{
					ci::gl::Texture texture;
					ci::Vec2f position;
				};

				std::vector<EmailTex> emailsTextures;


			public:
				static const int POPUP_CLOSED = 1;
				ci::Font inputFont;
				ci::Font addEmailFont;
				Popup(kubik::config::PhotoboothSettingsRef settings);

				void show();				
				void hide(EventGUIRef& event);
				void addEmailHandler(EventGUIRef& event);
				void draw();	
				void drawBackgrounds();				
			};
		}
	}
}