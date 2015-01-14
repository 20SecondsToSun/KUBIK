#pragma once
#include "Types.h"
using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class IButton : public IDrawable
	{
	public:
		IButton(Rectf rectf)
		{
			this->shift = Vec2f(rectf.x1, rectf.y1);
			setButtonArea(rectf);	
			isTexture = false;
		}

		IButton(Texture tex, Vec2f shift)
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

		Vec2f getPosition() const
		{
			return Vec2f(buttonArea.x1, buttonArea.y1);
		}

		float getWidth() const
		{
			return buttonArea.x2 - buttonArea.x1;
		}

		float getHeight() const
		{
			return buttonArea.y2 - buttonArea.y1;
		}

		Rectf getButtonArea()
		{
			return buttonArea;
		}

		virtual void mouseUpHandler( Vec2i vec)
		{
			if(buttonArea.contains(vec))		
				mouseUpSignal(*this);		
		}

		ButtonSignal mouseUpSignal;

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

	typedef shared_ptr<IButton> IButtonRef;

}