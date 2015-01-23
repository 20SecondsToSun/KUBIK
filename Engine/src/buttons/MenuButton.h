#pragma once

#include "gui/TextButton.h"
#include "GameChoosedEvent.h"
#include "Types.h"

namespace kubik
{
	class MenuButton: public TextButton
	{
	public:
		MenuButton(Rectf rectf, string text, Font font, GameId gameID)
			:TextButton(rectf, text, font)
		{	
			event = GameChoosedEventRef(new GameChoosedEvent(gameID));
		}
	};

	typedef shared_ptr<MenuButton> MenuButtonRef;
}