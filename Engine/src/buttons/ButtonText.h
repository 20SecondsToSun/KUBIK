#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "IButton.h"
#include "TextTools.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class ButtonText: public IButton
{
public:
	typedef boost::signals2::signal<void(ButtonText&)> ButtonSignal;
	
	ButtonSignal mouseUpSignal;

	ButtonText(Rectf rectf, string text, Font font):IButton(rectf)
	{
		this->font = font;
		this->text = text;
	}

	ButtonText(ci::gl::Texture tex, ci::Vec2f pos, string text, Font font):IButton(tex, pos)
	{
		this->font = font;
		this->text = text;
	}

	virtual void mouseUpHandler( Vec2i vec)
	{
		if(buttonArea.contains(vec))
			mouseUpSignal(*this);
	}
	
	void draw()
	{
		IButton::draw();

		Texture textTex = textTools().getTextField(text, &font, ColorA(1,0,0,1));
		gl::pushMatrices();			
			float shiftX = (buttonArea.getWidth() - textTex.getWidth()) * 0.5f;
			float shiftY = (buttonArea.getHeight() - textTex.getHeight()) * 0.5f;
			gl::translate(shift);
			gl::translate(shiftX, shiftY);
			gl::draw(textTex);
		gl::popMatrices();
	}

protected:

	Font	font;
	string  text;
};