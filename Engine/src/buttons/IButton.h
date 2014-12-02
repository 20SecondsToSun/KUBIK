#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class IButton
{
public:
	IButton(Rectf rectf)
	{
		setButtonArea(rectf);	
		isTexture = false;
	}

	IButton(ci::gl::Texture tex, ci::Vec2f shift)
	{
		this->tex = tex;
		this->shift = shift;

		setButtonArea(Rectf(shift.x, shift.y, shift.x + tex.getWidth(), shift.y + tex.getHeight()));	
		isTexture = true;
	}	

	void draw()
	{
		if(isTexture)
		{
			gl::draw(tex, shift);
		}
		else
			drawSolidRect(buttonArea);
	}

protected:
	Rectf buttonArea;
	bool isTexture;
	Vec2f shift;
	Texture tex;

	void setButtonArea(Rectf rectf)
	{
		buttonArea =  rectf;
	}

};