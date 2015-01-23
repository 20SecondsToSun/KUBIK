#pragma once
#pragma once

#include "gui/SimpleButton.h"
#include "TextTools.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class TextButton: public SimpleButton
	{
	public:	

		TextButton(Rectf rectf, string text, Font font):SimpleButton(rectf), font(font), text(text)
		{
			textTex = textTools().getTextField(text, &font, ColorA(1,0,0,1));
			shiftX  = (buttonArea.getWidth() - textTex.getWidth()) * 0.5f;
			shiftY  = (buttonArea.getHeight() - textTex.getHeight()) * 0.5f;
		}	

		void drawLayout()
		{
			SimpleButton::drawLayout();

			gl::translate(shiftX, shiftY);
			gl::draw(textTex);			
		}
	
	protected:
		Font	font;
		string  text;		
		Texture textTex;
		float   shiftX, shiftY;
	};

	typedef shared_ptr<TextButton> TextButtonRef;
}