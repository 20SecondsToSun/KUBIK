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
		typedef boost::signals2::signal<void(Button&)> ButtonSignal;

		ButtonSignal mouseUpSignal;

		Button(Rectf rectf):IButton(rectf)
		{

		}

		Button(Texture tex, Vec2f pos):IButton(tex, pos)
		{

		}

		virtual void mouseUpHandler( Vec2i vec)
		{
			if(buttonArea.contains(vec))		
				mouseUpSignal(*this);		
		}
	};

	typedef shared_ptr<Button> ButtonRef;
}