#pragma once

#include "IDrawable.h"

namespace kubik
{
	namespace config
	{
		class Logo:public IDrawable
		{
		public:
			Logo(Vec2i position, Texture tex):bckgrndColor(Color::hex(0x242135)),
				   blockWidth(245), blockHeight(170)				 
			{
				 setIcon(tex);
				 setPosition(position);
			}

			virtual void draw()
			{
				gl::pushMatrices();
				gl::translate(position);
				gl::color(bckgrndColor);
				gl::drawSolidRect(Rectf(0, 0, blockWidth, blockHeight));
				gl::color(Color::white());
				gl::draw(icon, iconPosition);
				gl::popMatrices();
			}			
			void setIcon(Texture tex)
			{
				icon = tex;
				iconPosition =  Vec2f(0.5 * (blockWidth - icon.getWidth()), 0.5 * (blockHeight - icon.getHeight()));
			}

		private:
			Vec2i iconPosition;
			Texture icon;
			Color bckgrndColor;
			int blockWidth, blockHeight;
		};

		typedef std::shared_ptr<Logo> LogoRef;
	}
}