#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ButtonText.h"
#include "Types.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class MenuButton: public ButtonText
	{
	public:
		typedef boost::signals2::signal<void(MenuButton&)> ButtonSignal;	
		ButtonSignal mouseUpSignal;

		MenuButton(game::id gameId, Rectf rectf, string text, Font font):ButtonText(rectf, text, font)
		{		
			this->gameId = gameId;
		}

		game::id getGameId()
		{
			return gameId;
		}

		void mouseUpHandler( Vec2i vec)
		{
			if(buttonArea.contains(vec))
				mouseUpSignal(*this);
		}

	private:

		game::id gameId;	
	};
}