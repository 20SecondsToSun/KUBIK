#pragma once
#include "Utils.h"
#include "PhotoboothSettings.h"
#include "gui/Sprite.h"
#include "gui/EventGUI.h"
#include "gui/ImageButtonSprite.h"
#include "VirtualKeyboard.h"
#include "states/sharing/social/Popup.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class EmailPopup>EmailPopupRef;

			class EmailPopup : public Popup
			{
				static const int MAX_EMAIL_TO_SEND = 3;

				ci::Anim<float>  alphaError;
				ci::ColorA addEmailFontColor;
				ci::gl::Texture  addEmailIcon, borderIcon, borderIconRed;
				ci::Vec2f borderIconPos, initAddEmailPosition,	shiftEmailPosition;

				kubik::ImageButtonSpriteRef addEmailBtn;

				struct EmailTex
				{
					ci::gl::Texture texture;
					ci::Vec2f position;
				};

				std::vector<EmailTex> emailsTextures;
				std::vector<std::string> emails;

				virtual void showAnimComplete() override;
				virtual void initVirtualKeyboard() override;

				void drawInputField();				
				void drawAddedEmails();
				void clearEmails();
				bool handleInputField();
				void showRedFocusStroke();
				void addEmailToList();				
				bool showAddEmail;	
				void addEmailHandler(EventGUIRef& event);
				void sendEmailHandler();

				ci::Font addEmailFont;

			public:								
				EmailPopup(kubik::config::PhotoboothSettingsRef settings);
				virtual void show() override;
				virtual void hide(EventGUIRef& event) override;				
				virtual void draw() override;				
			};
		}
	}
}