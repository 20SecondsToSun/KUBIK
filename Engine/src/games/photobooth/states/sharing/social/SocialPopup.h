#pragma once
#include "Utils.h"
#include "PhotoboothSettings.h"
#include "gui/Sprite.h"
#include "gui/EventGUI.h"
#include "gui/ImageButtonSprite.h"
#include "VirtualKeyboard.h"
#include "states/sharing/social/Popup.h"
#include "services/social/Social.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	std::shared_ptr<class SocialPopup> SocialPopupRef;

			class SocialPopup : public Popup
			{
			protected:
				virtual void createSocialContext() = 0;
				virtual void showAnimComplete() override;
				virtual void initVirtualKeyboard() override;				
				void postingStartHandler();
				void postingCompleteHandler();
				void disconnectSignals();

				SocShareRef social;
				ci::signals::connection hideSignalCon, postingStartSignalCon, postingCompleteSignalCon, postingErrorSignalCon;

			public:
				SocialPopup(kubik::config::PhotoboothSettingsRef settings);

				virtual void hide(EventGUIRef& event) override;
				virtual void show() override;

				virtual void draw()  override;
				virtual void kill()  override;
				virtual void close() override;				
			};
		}
	}
}