#pragma once

#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "TextTools.h"
#include "model/PhotoStorage.h"
#include "gui/ImageButtonSprite.h"
#include "states/sharing/FinalPhotoTemplate.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class PhotoSharing> PhotoSharingRef;

			class PhotoSharing:public IPhotoboothLocation
			{
				PhotoStorageRef  photoStorage;		
				float startServiceButtonY, leftBlockX;
				bool qrCodeFlag;
				ci::gl::Texture texTitle, sharefon, qrTitle;	
				ci::Vec2f texTitlePos, sharefonPos, qrTitlePos;	

				FinalPhotoTemplate finalPhotoTemplate;
				ImageButtonSpriteRef emailBtn,fbBtn, vkBtn, twBtn, againBtn, allAppBtn;

				void drawServiceButtons();		
				void drawFinalPhoto() ;
				void againBtnHandler(EventGUIRef& event);
				void allAppBtnHandler(EventGUIRef& event);
				void emailBtnHandler(EventGUIRef& event);		
				void fbBtnHandler(EventGUIRef& event);
				void vkBtnHandler(EventGUIRef& event);
				void twBtnHandler(EventGUIRef& event);

			public:
				PhotoSharing(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage);		
				void reset(PhotoboothSettingsRef settings);
				void start();
				void stop();
				void update();
				void draw();
			};	
		}
	}
}