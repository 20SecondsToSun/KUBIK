#pragma once

#include "IDispatcher.h"

namespace kubik
{
	namespace config
	{
		class Logo:public IDispatcher
		{
		public:
			Logo(ConfigSettingsRef configSettings, Vec2i position)
				:bckgrndColor(Color::hex(0x242135)),
				   blockWidth(245), blockHeight(170)				 
			{
				
				 setIcon(configSettings->getTexture("logoIcon"));
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

			float getWidth()
			{
				return blockWidth;
			}

			void animateToMiniState(EaseFn eFunc, float time)
			{
				bckgrndColor = Color::hex(0x0d0917);
				iconPosition += Vec2f(-37, 0);
			}

			void animateToMaxState(EaseFn eFunc, float time)
			{
				bckgrndColor = Color::hex(0x242135);
				iconPosition += Vec2f(37, 0);
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