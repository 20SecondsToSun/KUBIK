#pragma once

#include "ButtonText.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{	
	class PhotoButton: public IButton
	{
		Surface thumb;
		bool _isSelect;
		int id;		

	public:
		typedef boost::signals2::signal<void(PhotoButton&)> ButtonSignal;
		ButtonSignal mouseUpSignal;
		
		PhotoButton(int id, Surface surf, Vec2f vec):IButton(Rectf(vec.x, vec.y, vec.x + surf.getWidth(), vec.y + surf.getHeight()))
		{		
			this->id = id;
			_isSelect = false;
			thumb = surf;
		}

		int getFilterId()
		{
			return id;
		}

		void mouseUpHandler(Vec2i vec)
		{
			if(buttonArea.contains(vec))
			{
				_isSelect = !_isSelect;
				mouseUpSignal(*this);
			}
		}

		void draw()
		{
			gl::draw(thumb, shift);

			if(_isSelect)
				gl::color(ColorA(0.0f, 0.0f, 0.0f, 0.6f));
			else
				gl::color(ColorA(1.0f, 1.0f, 1.0f, 0.6f));

			IButton::draw();

			gl::color(Color::white());

		/*	Texture textTex = textTools().getTextField(text, &font, ColorA(1, 0, 0, 1));
			gl::pushMatrices();			
			float shiftX = (buttonArea.getWidth() - textTex.getWidth()) * 0.5f;
			float shiftY = (buttonArea.getHeight() - textTex.getHeight()) * 0.5f;
			gl::translate(shift);
			gl::translate(shiftX, shiftY);

			gl::draw(textTex);
			gl::popMatrices();*/
		}

		void setSelection(bool value)
		{
			_isSelect = value;
		}

		bool isSelect()
		{
			return _isSelect;
		}
	};

	typedef shared_ptr<PhotoButton> PhotoButtonRef;
}