#pragma once

#include "states/IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "TextTools.h"
#include "model/PhotoStorage.h"
#include "gui/ImageButtonSprite.h"
#include "states/sharing/FinalPhotoTemplate.h"
#include "states/sharing/QrCode.h"
#include "states/sharing/social/Popup.h"
#include "states/sharing/social/EmailPopup.h"
#include "states/sharing/social/VkontaktePopup.h"
#include "states/sharing/social/FacebookPopup.h"
#include "states/sharing/social/TwitterPopup.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class PhotoSharing> PhotoSharingRef;

			class PhotoSharing : public IPhotoboothLocation
			{
				float startServiceButtonY, leftBlockX;
				bool qrCodeFlag;

				enum locationState
				{
					ANIM_HIDE,
					TEMPLATE_CHOOSE,
					POPUP
				}
				state;

				PopupRef			popup;
				EmailPopupRef		emailpopup;
				VkontaktePopupRef	vkpopup;
				FacebookPopupRef	fbpopup;
				TwitterPopupRef		twpopup;

				PhotoStorageRef		photoStorage;
				ci::gl::Texture		sharefon;
				ci::Vec2f			sharefonPos, qrTitlePos, titlePos;
				ci::Anim<float>		alphaAnim, leftBlockAnimateX;
				ci::Anim<ci::Vec2f> sharefonPosAnim;

				std::vector<ImageButtonSpriteRef> serviceBtns;

				FinalPhotoTemplate   finalPhotoTemplate;
				ImageButtonSpriteRef emailBtn, fbBtn, vkBtn, twBtn, againBtn, allAppBtn;

				QrCodeRef qrcode, qrCodeObject;
				QrCodeNullRef qrCodeNullObject;

				SimpleSpriteButtonRef simple;

				int uploadPhotoID;

				void hideAnimComplete();
				void drawServiceButtons();
				void drawFinalPhoto();
				void againBtnHandler(EventGUIRef& event);
				void allAppBtnHandler(EventGUIRef& event);
				void emailBtnHandler(EventGUIRef& event);
				void fbBtnHandler(EventGUIRef& event);
				void vkBtnHandler(EventGUIRef& event);
				void twBtnHandler(EventGUIRef& event);
				void initShowAnim();
				void disconnectEventHandlers();
				void connectHandlers();
				void popupClosed();

				void photoUploadCompleteHandler(const std::string& photo_id, const std::string& link);
				void photoUploadErrorHandler();

			public:
				PhotoSharing(config::PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage);

				virtual void reset(config::PhotoboothSettingsRef settings) override;
				virtual void start() override;
				virtual void stop() override;
				virtual void update() override;
				virtual void draw()	override;
				virtual void stopAllTweens() override;
				void showPopup();	
				void shareCompleteHandler(SharingType i, const std::string& linkToPost);
			};
		}
	}
}
