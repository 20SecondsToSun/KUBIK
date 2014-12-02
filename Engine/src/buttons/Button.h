#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "IButton.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class Button: public IButton
{
public:
	typedef boost::signals2::signal<void(Button&)> ButtonSignal;
	
	ButtonSignal mouseUpSignal;

	Button(Rectf rectf):IButton(rectf)
	{		
	
	}

	Button(ci::gl::Texture tex, ci::Vec2f pos):IButton(tex, pos)
	{		
	
	}

	virtual void mouseUpHandler( Vec2i vec)
	{
		if(buttonArea.contains(vec))
			mouseUpSignal(*this);
	}

};