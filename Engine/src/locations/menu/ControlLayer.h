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
		typedef shared_ptr<class ControlLayer> ControlLayerRef;

		class ControlLayer : public Sprite
		{
			ci::gl::Texture tex;
			ImageButtonSpriteRef btn;
			SimpleSpriteButtonRef settingsbtn;

		public:
			static const int CLOSE_GAME = 0; 
			static const int OPEN_SETTINGS = 1;

			ControlLayer(ISettingsRef settings):tex(settings->getTexture("menuButton"))
			{
				btn = ImageButtonSpriteRef(new ImageButtonSprite(tex, ci::Vec2f(44.0f, 44.0f)));
				createControlsButtons();
			}

			void showBackButton()
			{
				addChild(btn);
			}

			virtual void activateListeners()
			{				
				btn->connectEventHandler(&ControlLayer::close, this);
			}

			void hideBackButton()
			{
				removeChild(btn);
			}

			void close(EventGUIRef& event)
			{
				callback(CLOSE_GAME);
			}

			virtual void unActivateListeners()
			{				
				btn->disconnectEventHandler();
			}

			virtual void draw()
			{
				Sprite::draw();
			}

			void createControlsButtons()
			{
				Rectf setButtonArea = Rectf(250.0f, 0.0f, 650.0f, 25.0f);
				settingsbtn = SimpleSpriteButtonRef(new SimpleSpriteButton(setButtonArea));
				settingsbtn->connectEventHandler(&ControlLayer::startSettings, this);
				addChild(settingsbtn);
			}

			void startSettings(EventGUIRef& event)
			{
				callback(OPEN_SETTINGS);
			}

			void clear()
			{
				settingsbtn->disconnectEventHandler();
				btn->disconnectEventHandler();
				removeChild(settingsbtn);
				removeChild(btn);
			}
		};
	}
}