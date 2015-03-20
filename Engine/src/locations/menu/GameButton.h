#pragma once
#include "gui/EventGUI.h"
#include "gui/SimpleSpriteButton.h"
#include "MenuSettings.h"

namespace kubik
{
	namespace menu
	{
		typedef std::shared_ptr<class GameButton> GameButtonRef;

		class GameButton :public SimpleSpriteButton
		{
			ci::gl::Texture texture;
		
		public:
			GameButton(const GameData& data);
			virtual void drawLayout() override;
		};
	}
}