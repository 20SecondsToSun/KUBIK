#pragma once
#include "Types.h"
#include "Sprite.h"
#include "gui/EventGUI.h"
#include "Utils.h"

namespace kubik
{
	class SimpleSpriteButton : public Sprite
	{
	public:		
		SimpleSpriteButton(const ci::Rectf& rect, EventGUIRef _event = EventGUIRef(new EventGUI()));
		SimpleSpriteButton(float width, float height, const ci::Vec2f& position, EventGUIRef _event = EventGUIRef(new EventGUI()));
		SimpleSpriteButton(const ci::Vec2f& position0, const ci::Vec2f& position);

		virtual void mouseUp(ci::app::MouseEvent &e);
		virtual void mouseUpHandler(const ci::Vec2i& vec);	

		bool inButtonField(const ci::Vec2i& pos);		
		float getAlpha() const;
		float getWidth() const;
		float getHeight() const;

		ci::Vec2f getPosition() const;
		ci::Vec2f getSize() const;
		ci::Rectf getButtonArea() const;

		void drawLayout() override;
		void setButtonArea(const ci::Rectf& rectf);
		void setButtonArea1(const ci::Rectf& rect);
		void setColor(const ci::Color& color);
		void setAlpha(float alpha);
		void showAnimate(float startA, float endA, float time, float delay);
		void showPositionAnimate(const ci::Vec2f& endA, float time, float delay);
		void stopAllTweens();		

	protected:
		bool bgVisible;
		ci::Rectf buttonArea;		
		ci::ColorA color;	
		ci::Anim<float> alpha;
		ci::Anim<ci::Vec2f> animPosition;
	};

	typedef shared_ptr<SimpleSpriteButton> SimpleSpriteButtonRef;
}