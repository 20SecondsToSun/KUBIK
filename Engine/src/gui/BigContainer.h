#pragma once
#include "CompositeDispatcher.h"
#include "ButtonField.h"
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
	class BigContainer :public CompositeDispatcher
	{
	public:
		ButtonFieldRef field;	

		BigContainer(Rectf rect, Color color):color(color)
		{	
			area = Rectf(0, 0, rect.getWidth(),rect.getHeight()); 
			setPosition(Vec2f(rect.x1, rect.y1));	

			field  = ButtonFieldRef(new ButtonField(Rectf(10, 10, 600, 600), Color(0,1,0)));
			field->addMouseUpListener(&BigContainer::click2, this);
			addChild(field);
		}

		void click2(EventRef event)
		{
			//console()<<"conteiner ::: "<<event->getMsg()<<endl;
			mouseUpSignal(event);
		}

		virtual void draw()
		{
			gl::color(color);
			gl::pushMatrices();
			gl::translate(position);
			gl::drawSolidRect(area);
			gl::popMatrices();
			gl::color(Color::white());

			CompositeDispatcher::draw();
		}

		//virtual void mouseUp(MouseEvent &e)
		//{
		//	if((area + position).contains(e.getPos()))
		//			mouseUpSignal(event);
		//}

	protected:
		Rectf area;
		Color color;
	};
	typedef shared_ptr<BigContainer> BigContainerRef;	
}