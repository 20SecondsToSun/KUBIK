#pragma once
#include "PozaSettings.h"
#include "TextTools.h"
#include "gui/ImageButtonSprite.h"
#include "main/FinalPhotoTemplate.h"
#include "main/QrCode.h"
#include "main/social/Popup.h"
#include "main/social/EmailPopup.h"
#include "main/social/VkontaktePopup.h"
#include "main/social/FacebookPopup.h"
#include "main/social/TwitterPopup.h"
#include "main/IGameLocation.h"
#include "config/StatCollector.h"

using namespace kubik::games::photobooth;

namespace kubik
{
	namespace games
	{		
		typedef	std::shared_ptr<class SocialLocation> SocialLocationRef;

		class SocialLocation : public IGameLocation
		{
			float startServiceButtonY, leftBlockX;
			bool qrCodeFlag;
			int uploadPhotoID;

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
			config::StatCollectorRef statSettings;				

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
			SocialLocation(config::ISettingsRef settings, config::StatCollectorRef statSettings, PhotoStorageRef  photoStorage);

			void reset(config::ISettingsRef settings, config::StatCollectorRef statsettings);
			virtual void start()		 override;
			virtual void stop()			 override;
			virtual void update()		 override;
			virtual void draw()			 override;
			virtual void stopAllTweens() override;

			void showPopup();
			void shareCompleteHandler(SharingType i, const std::string& linkToPost);
		};		
	}
}
