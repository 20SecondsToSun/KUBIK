#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "IButton.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class MenuButton: public IButton
{
public:
	typedef boost::signals2::signal<void(MenuButton&)> ButtonSignal;	
	ButtonSignal mouseUpSignal;

	MenuButton(int gameId, Rectf rectf):IButton(rectf)
	{		
		this->gameId = gameId;
	}

	int getGameId()
	{
		return gameId;
	}

	void mouseUpHandler( Vec2i vec)
	{
		if(buttonArea.contains(vec))
			mouseUpSignal(*this);
	}

private:

	int gameId;	
};