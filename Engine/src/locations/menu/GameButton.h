#pragma once
#include "gui/EventGUI.h"
#include "gui/SimpleSpriteButton.h"
#include "MenuSettings.h"
#include "GameChoosedEvent.h"

using namespace kubik::config;

namespace kubik
{
	namespace menu
	{
		typedef std::shared_ptr<class GameButton> GameButtonRef;

		class GameButton :public SimpleSpriteButton
		{
			ci::gl::Texture texture;
			ci::gl::Texture background;
			ci::gl::Texture title;

			ci::Vec2f backgroundPosition;
			ci::Vec2f titlePosition;

		public:
			GameButton(const GameData& data,
				const ci::gl::Texture& texture,
				const ci::Vec2f& backgroundPosition,
				const ci::gl::Texture& title,
				const ci::Vec2f& titlePosition);
			virtual void drawLayout() override;
		};
	}
}