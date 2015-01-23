#pragma once
#include "Dispatcher.h"
#include "cinder/ImageIO.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci::signals;
using namespace ci::app;
using namespace std;
using namespace ci;

namespace kubik
{
	class Button :public Dispatcher
	{
	public:
		Button(Rectf rect, Color color):color(color)
		{	
			area = Rectf(0, 0, rect.getWidth(),rect.getHeight()); 
			setLocalPosition(Vec2f(rect.x1, rect.y1));
		}

		/*virtual void setPosition(ci::Vec2i position)		
		{
			area = Rectf(0, 0, rect.getWidth(),rect.getHeight()); 
			setPosition(Vec2f(rect.x1, rect.y1));		
		}*/

		virtual void draw()
		{
			gl::color(color);
			gl::pushMatrices();
			gl::translate(localPosition + position);
			gl::drawSolidRect(area);
			gl::popMatrices();
			gl::color(Color::white());
		}

		virtual void mouseUp(MouseEvent &e)
		{
			//console()<<"seeet"<<endl;
			if((area + localPosition + position).contains(e.getPos()))
					mouseUpSignal(event);
		}

		//virtual void mouseUpHandler(ci::Vec2i pos)
		//{	
			//mouseUpSignal(event);
		//}	

	protected:
		Rectf area;
		Color color;
	};
	typedef shared_ptr<Button> ButtonRef;	
}