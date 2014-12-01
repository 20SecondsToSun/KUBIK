#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class MenuButton
{
public:
	typedef boost::signals2::signal<void(MenuButton&)> ButtonSignal;	
	
	ButtonSignal mouseUpSignal;

	MenuButton(int gameId, Rectf rectf)
	{
		this->gameId = gameId;
		setButtonArea(rectf);	
	}	

	void draw()
	{
		drawSolidRect(buttonArea);
	}

	void mouseUpHandler( Vec2i vec)
	{
		if(buttonArea.contains(vec))
			mouseUpSignal(*this);		
	}

	int getGameId( )
	{
		return gameId;
	}

private:

	Rectf buttonArea;
	int gameId;

	void setButtonArea(Rectf rectf)
	{
		buttonArea =  rectf;
	}	
};