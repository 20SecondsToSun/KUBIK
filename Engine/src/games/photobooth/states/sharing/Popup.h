#pragma once
#include "Utils.h"
#include "gui/Sprite.h"
#include "gui/EventGUI.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			class Popup: public Sprite
			{
				ci::Anim<float> alphaAnim;
				void showAnimComplete();
				void hideAnimComplete();

			public:
				static const int POPUP_CLOSED = 1;
				Popup();

				void show();				
				void hide(EventGUIRef& event);
				void draw();				
			};
		}
	}
}