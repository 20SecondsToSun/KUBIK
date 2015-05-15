#pragma once
#include "states/sharing/social/SocialPopup.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class TwitterPopup> TwitterPopupRef;

			class TwitterPopup : public SocialPopup
			{
				void createSocialContext() override;

			public:
				TwitterPopup(kubik::config::PhotoboothSettingsRef settings);
			};
		}
	}
}
//#include "Utils.h"
//#include "PhotoboothSettings.h"
//#include "gui/Sprite.h"
//#include "gui/TextField.h"
//#include "gui/EventGUI.h"
//#include "gui/ImageButtonSprite.h"
//#include "VirtualKeyboard.h"
//#include "states/sharing/social/Popup.h"
//#include "services/social/Twitter.h"
//
//namespace kubik
//{
//	namespace games
//	{
//		namespace photobooth
//		{
//			typedef	std::shared_ptr<class TwitterPopup> TwitterPopupRef;
//
//			class TwitterPopup : public Popup
//			{	
//				void showAnimComplete() override;
//				void initVirtualKeyboard() override;
//				void keyTouchHandler();
//				void sendHandler();		
//				void postingCompleteHandler();
//				void disconnectListeners();				
//				void postingErrorHandler();
//				void gettingAuthURLCompleteHandler();				
//				
//				void connectKeyboard();
//				void disconnectKeyboard();		
//				void closePopupHandler(EventGUIRef& event);
//				void tryAgainHandler(EventGUIRef& event);				
//				void sendButtonHandler(EventGUIRef& event);
//				void loginTouchHandler(EventGUIRef& event);
//				void passTouchHandler(EventGUIRef& event);
//
//				kubik::TwitterRef twitter; 
//				gl::Texture socialTexture, errorBg, okPopup;
//				ci::Vec2f socialTexturePosition;
//
//				enum SocialPopupState
//				{
//					SOCIAL_POSTING,
//					SOCIAL_ERROR,
//					SOCIAL_OK,
//					SOCIAL_DRAWING,
//					AUTHORIZATION
//				}
//				state;
//
//				void setState(const SocialPopupState& value);
//
//				kubik::gui::TextFieldRef loginField, passField, activeField;
//
//				SimpleSpriteButtonRef enterBtn;
//
//				WebCore*		 mWebCorePtr;
//				WebView*		 mWebViewPtr;
//				ci::gl::Texture  mWebTexture;
//
//				void chromeUpdate();
//				void chromeDraw();
//
//			public:				
//				TwitterPopup(kubik::config::PhotoboothSettingsRef settings);
//
//				void show() override;
//				void hide(EventGUIRef& event) override;
//				void draw() override;
//			};
//		}
//	}
//}