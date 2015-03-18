#pragma once
#include "Types.h"
#include "Sprite.h"
#include "gui/EventGUI.h"
#include "Utils.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

namespace kubik
{
	class SimpleSpriteButton : public Sprite
	{
	public:
		string name;
		SimpleSpriteButton(Rectf rect, EventGUIRef _event = EventGUIRef(new EventGUI()))
			:Sprite(),
			color(Color::white()),
			bgVisible(true), alpha(1.0f), animPosition(Vec2f::zero())
		{
			buttonArea = Rectf(0, 0, rect.getWidth(),rect.getHeight()); 
			setPosition(Vec2f(rect.x1, rect.y1));
		
			event = _event;
		}

		SimpleSpriteButton(float width, float height, Vec2f position, EventGUIRef _event = EventGUIRef(new EventGUI()))
			:Sprite(), color(Color::white()), bgVisible(true), alpha(1.0f), animPosition(Vec2f::zero())
		{
			buttonArea = Rectf(0, 0, width, height); 
			setPosition(position);
		
			event = _event;
		}

		SimpleSpriteButton(Vec2f position0, Vec2f position) :Sprite(), color(Color::white()), bgVisible(true), alpha(1.0f), animPosition(Vec2f::zero())
		{
			buttonArea = Rectf(Vec2f::zero(), position0); 
			setPosition(position);
		
			event = EventGUIRef(new EventGUI());
		}
		
		virtual void mouseUp(ci::app::MouseEvent &_event)
		{
			//console()<<"override"<<endl;
			if(inButtonField(_event.getPos()))
			{
				//console()<<"in button "<<endl;
				//console()<<"lock"<<lock<<endl;
				//mEventHandler(event);
				Sprite::mouseUp(_event);
			}
		}

		bool inButtonField(Vec2i pos)
		{			
			return (buttonArea + getGlobalPosition()).contains(pos);			
		}

		void drawLayout()
		{
			gl::color(ColorA(color.r, color.g, color.b, alpha));
			gl::drawSolidRect(buttonArea);
			gl::color(Color::white());
			Sprite::drawLayout();
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

		ci::Vec2f getSize()
		{
			return ci::Vec2f( getWidth(), getHeight());
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

		void setButtonArea1(Rectf rect)
		{
			buttonArea = Rectf(0, 0, rect.getWidth(),rect.getHeight()); 
			setPosition(Vec2f(rect.x1, rect.y1));	
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
			this->alpha = alpha;
			color = Utils::colorAlpha(color, alpha);
			Sprite::setAlpha(alpha);
		}

		void showAnimate(float startA, float endA, float time, float delay)
		{
			alpha = 0.0f;
			timeline().apply(&alpha, startA, endA, time, EaseInOutCubic()).delay(delay);
		}

		void showPositionAnimate(const ci::Vec2f& endA, float time, float delay)
		{
			animPosition = endA;
			timeline().apply(&animPosition, Vec2f::zero(), time, EaseOutCubic()).delay(delay);
		}

		void stopAllTweens()
		{
			animPosition.stop();
			alpha.stop();
		}

	protected:
		Rectf buttonArea;
		bool bgVisible;	
		ColorA color;	
		ci::Anim<float> alpha;
		ci::Anim<ci::Vec2f> animPosition;
	};

	typedef shared_ptr<SimpleSpriteButton> SimpleSpriteButtonRef;
}