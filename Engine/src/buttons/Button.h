#pragma once

#include "IButton.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class Button: public IButton
	{
	public:	

		Button(Rectf rectf):IButton(rectf)
		{
			event = EventRef(new Event("buttonClick"));
		}

		Button(Texture tex, Vec2f pos):IButton(tex, pos)
		{
			event = EventRef(new Event("buttonClick"));
		}	

		virtual void mouseUpHandler(Vec2i vec)
		{
			if(buttonArea.contains(vec))
			{
				mouseUpSignal(event);
			}
		}

		void changeTexture(Texture _tex)
		{
			tex = _tex;
		}

		EventRef event;
	};

	typedef shared_ptr<Button> ButtonRef;
}