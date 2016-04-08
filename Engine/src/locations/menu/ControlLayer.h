#pragma once

#include "MenuSettings.h"
#include "IScreen.h"
#include "gui/EventGUI.h"
#include "gui/Sprite.h"
#include "gui/ImageButtonSprite.h"

namespace kubik
{
	namespace menu
	{
		typedef std::shared_ptr<class ControlLayer> ControlLayerRef;

		class ControlLayer : public Sprite
		{
			ci::gl::Texture tex;
			ImageButtonSpriteRef btn;
			SimpleSpriteButtonRef settingsbtn;

		public:
			static const int CLOSE_GAME = 0;
			static const int OPEN_SETTINGS = 1;

			ControlLayer(config::ISettingsRef settings);

			virtual void activateListeners();
			virtual void unActivateListeners();
			virtual void draw();

			void showBackButton();
			void hideBackButton();
			void hideButtons();			
			void close(EventGUIRef& event);
			void createControlsButtons();
			void startSettings(EventGUIRef& event);
			void clear();
		};
	}
}