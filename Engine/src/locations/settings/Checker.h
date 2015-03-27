#pragma once
#include "gui/SimpleSpriteButton.h"
#include "CheckerEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class Checker> CheckerRef;

		class Checker: public SimpleSpriteButton
		{
		public:
			Checker(const ci::Rectf& rect, const IconPair& icons, ci::Color activeColor, ci::Color unActiveColor);
			
			virtual void mouseUp(ci::app::MouseEvent &_event);
			virtual void drawLayout();

			void setAlpha(float alpha);
			void setActive(bool isActive);
			void setFreeze();
			bool getValue() const;
			void setRadius(float radius);
			void setStartX(float x);
			void setFinishX(float x);
			void swapActive();
			void setBorderColorActive(const ci::ColorA& color);
			void setBorderColorUnActive(const ci::ColorA& color);

		protected:
			float sdvigX;
			float radius, startX, finishX;
			bool isActive;
			ci::ColorA color, borderColor, iconColor, activeBordercolor, unActiveBordercolor;
			ci::Color activeColor, unActiveColor;
			
			IconPair icons;
			ci::gl::Texture icon;
			ci::ColorA freezeColor;
		};
	}
}