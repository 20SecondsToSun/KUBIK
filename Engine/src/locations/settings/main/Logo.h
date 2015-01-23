#pragma once
#include "gui/Dispatcher.h"


namespace kubik
{
	namespace config
	{
		class Logo:public Dispatcher
		{
		public:
			Logo(ConfigSettingsRef configSettings, Vec2i position)
				:bckgrndColor(Color::hex(0x242135)),
				   blockWidth(245), blockHeight(170)				 
			{				
				 setIcon(configSettings->getTexture("logoIcon"));
				 setPosition(position);
			}

			virtual void drawLayout()
			{				
				gl::color(bckgrndColor);
				gl::drawSolidRect(Rectf(0, 0, blockWidth, blockHeight));
				gl::color(Color::white());
				gl::draw(icon, iconPosition);		
			}

			void setIcon(Texture tex)
			{
				icon = tex;
				saveIconPosition = iconPosition =  Vec2f(0.5 * (blockWidth - icon.getWidth()), 0.5 * (blockHeight - icon.getHeight()));
			}

			float getWidth()
			{
				return blockWidth;
			}

			void animateToMiniState(EaseFn eFunc, float time)
			{
				bckgrndColor = Color::hex(0x0d0917);
				iconPosition = saveIconPosition + Vec2f(-37, 0);
			}

			void animateToMaxState(EaseFn eFunc, float time)
			{
				bckgrndColor = Color::hex(0x242135);
				iconPosition = saveIconPosition;
			}

		private:
			Vec2i iconPosition, saveIconPosition;
			Texture icon;
			Color bckgrndColor;
			int blockWidth, blockHeight;
		};

		typedef std::shared_ptr<Logo> LogoRef;
	}
}