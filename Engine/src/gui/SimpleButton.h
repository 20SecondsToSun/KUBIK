#pragma once
#include "Types.h"
#include "Dispatcher.h"
#include "gui/EventGUI.h"
#include "Utils.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class SimpleButton : public Dispatcher
	{
	public:
		string name;
		SimpleButton(Rectf rect, EventGUIRef _event = EventGUIRef(new EventGUI())):Dispatcher(),color(Color::white()), bgVisible(true)
		{
			buttonArea = Rectf(0, 0, rect.getWidth(),rect.getHeight()); 
			setPosition(Vec2f(rect.x1, rect.y1));
		
			event = _event;
		}

		SimpleButton(float width, float height, Vec2f position, EventGUIRef _event = EventGUIRef(new EventGUI())):Dispatcher(), color(Color::white()), bgVisible(true)
		{
			buttonArea = Rectf(0, 0, width, height); 
			setPosition(position);
		
			event = _event;
		}

		SimpleButton(Vec2f position0, Vec2f position):Dispatcher(), color(Color::white()), bgVisible(true)
		{
			buttonArea = Rectf(Vec2f::zero(), position0); 
			setPosition(position);
		
			event = EventGUIRef(new EventGUI());
		}

		virtual void mouseUp(MouseEvent &e)
		{
			if(inButtonField(e.getPos()))
				mouseUpSignal(event);
		}

		bool inButtonField(Vec2i pos)
		{			
			return (buttonArea + getGlobalPosition()).contains(pos);			
		}

		void drawLayout()
		{
			gl::color(color);
			gl::drawSolidRect(buttonArea);
			gl::color(Color::white());
			Dispatcher::drawLayout();
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

		virtual void mouseUpHandler(Vec2i vec)
		{
		}

		void setButtonArea(Rectf rectf)
		{
			buttonArea = rectf;
		}

		void setColor(Color color)
		{
			this->color = color;
		}

		float getAlpha()
		{
			return alpha;
		}

		void setAlpha(float alpha)
		{
			color = Utils::colorAlpha(color, alpha);
			Dispatcher::setAlpha(alpha);
		}

	protected:
		Rectf buttonArea;
		bool bgVisible;	
		ColorA color;	
		
	};

	typedef shared_ptr<SimpleButton> SimpleButtonRef;
}