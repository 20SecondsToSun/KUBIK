#pragma once
#include "CompositeDispatcher.h"
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
	class ButtonField :public CompositeDispatcher
	{
	public:
		ButtonRef button1, button2;

		ButtonField(Rectf rect, Color color):color(color)
		{
			event = EventRef(new Event());

			area = Rectf(0, 0, rect.getWidth(),rect.getHeight()); 
			setLocalPosition(Vec2f(rect.x1, rect.y1));			

			button2 = ButtonRef(new Button(Rectf(100, 100, 200, 200), Color(1,0,0)));					
			button2->addMouseUpListener(&ButtonField::click2, this);
			addChild(button2);	

			button1 = ButtonRef(new Button(Rectf(300, 300, 400, 400), Color(0,1,0)));					
			button1->addMouseUpListener(&ButtonField::click1, this);
			addChild(button1);	

			//button->addMouseUpListener(&_TBOX_PREFIX_App::mouseUp, this);
		}

		void click1(EventRef event)
		{
			event->setMsg("btn1");
			mouseUpSignal(event);
		}

		void click2(EventRef event)
		{
			event->setMsg("btn2");
			mouseUpSignal(event);
		}
		

		virtual void draw()
		{
			gl::color(color);
			gl::pushMatrices();
			gl::translate(localPosition + position);
			gl::drawSolidRect(area);
			gl::popMatrices();
			gl::color(Color::white());

			CompositeDispatcher::draw();
		}

		virtual void mouseUp(MouseEvent &e)
		{
			if((area + position).contains(e.getPos()))
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
	typedef shared_ptr<ButtonField> ButtonFieldRef;	
}