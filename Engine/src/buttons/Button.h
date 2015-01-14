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

		}

		Button(Texture tex, Vec2f pos):IButton(tex, pos)
		{

		}		
	};

	typedef shared_ptr<Button> ButtonRef;
}