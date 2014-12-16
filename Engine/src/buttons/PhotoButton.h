#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "ButtonText.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{	
	class PhotoButton: public ButtonText
	{
		bool _isSelect;

	public:

		typedef boost::signals2::signal<void(PhotoButton&)> ButtonSignal;

		ButtonSignal mouseUpSignal;

		PhotoButton(int filterId, Rectf rectf, string text, Font font):ButtonText(rectf, text, font)
		{		
			this->filterId = filterId;
			_isSelect = false;
		}

		int getFilterId()
		{
			return filterId;
		}

		void mouseUpHandler( ci::Vec2i vec)
		{
			if(buttonArea.contains(vec))
			{
				_isSelect = !_isSelect;
				mouseUpSignal(*this);
			}
		}

		void draw()
		{
			if(_isSelect)
				gl::color(Color::black());
			else
				gl::color(Color::white());

			IButton::draw();

			gl::color(Color::white());

			Texture textTex = textTools().getTextField(text, &font, ColorA(1, 0, 0, 1));
			gl::pushMatrices();			
			float shiftX = (buttonArea.getWidth() - textTex.getWidth()) * 0.5f;
			float shiftY = (buttonArea.getHeight() - textTex.getHeight()) * 0.5f;
			gl::translate(shift);
			gl::translate(shiftX, shiftY);

			gl::draw(textTex);
			gl::popMatrices();
		}

		void setSelection(bool value)
		{
			_isSelect = value;
		}

		bool isSelect()
		{
			return _isSelect;
		}

	private:

		int filterId;	
	};
}