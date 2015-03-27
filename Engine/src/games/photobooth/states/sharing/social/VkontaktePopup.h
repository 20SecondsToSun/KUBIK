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
			typedef	std::shared_ptr<class VkontaktePopup> VkontaktePopupRef;

			class VkontaktePopup : public Popup
			{	
				virtual void showAnimComplete() override;
				virtual void initVirtualKeyboard() override;
				void keyTouchHandler();

			public:				
				VkontaktePopup(kubik::config::PhotoboothSettingsRef settings);

				virtual void show() override;
				virtual void hide(EventGUIRef& event) override;
				virtual void draw() override;				
			};
		}
	}
}