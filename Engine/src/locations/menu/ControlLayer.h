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

		public:
			static const int CLOSE_GAME = 0;

			ControlLayer(ISettingsRef settings)
				:tex(settings->getTexture("menuButton"))
			{
				btn = ImageButtonSpriteRef(new ImageButtonSprite(tex, ci::Vec2f(44, 44)));
			}

			virtual void activateListeners()
			{
				btn->connectEventHandler(&ControlLayer::close, this);
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
				btn->draw();
			}
		};
	}
}