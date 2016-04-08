#pragma once

#include "gui/EventGUI.h"
#include "gui/SimpleSpriteButton.h"
#include "MenuSettings.h"
#include "GameChoosedEvent.h"

namespace kubik
{
	namespace menu
	{
		typedef std::shared_ptr<class GameButton> GameButtonRef;

		class GameButton :public SimpleSpriteButton
		{
			ci::gl::Texture iconTexture;
			ci::gl::Texture backgroundTexture;
			ci::gl::Texture titleTexture;

			ci::Vec2f backgroundPosition;
			ci::Vec2f titlePosition;
			ci::Vec2f iconPosition;

		public:
			GameButton(const config::GameData& data, config::AdditionalGameData adata);
			virtual void drawLayout() override;
		};
	}
}
