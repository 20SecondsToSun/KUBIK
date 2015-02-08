#pragma once
#include "gui/Sprite.h"

namespace kubik
{
	namespace config
	{
		class Logo:public Sprite
		{
		public:
			Logo(ConfigSettingsRef configSettings, Vec2i position)
				:bckgrndColor(ci::Color::hex(0x242135)),
				 blockWidth(245), 
				 blockHeight(170)				 
			{				
				 setIcon(configSettings->getTexture("logoIcon"));
				 setPosition(position);
			}

			virtual void drawLayout()
			{				
				gl::color(bckgrndColor);
				gl::drawSolidRect(ci::Rectf(0, 0, blockWidth, blockHeight));
				gl::color(ci::Color::white());
				gl::draw(icon, iconPosition);		
			}

			void setIcon(const ci::gl::Texture& tex)
			{
				icon = tex;
				saveIconPosition =
				iconPosition	 =  ci::Vec2f(0.5 * (blockWidth - icon.getWidth()),
											  0.5 * (blockHeight - icon.getHeight()));
			}

			float getWidth() const
			{
				return blockWidth;
			}

			void animateToMiniState(ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.9f)
			{
				bckgrndColor = ci::Color::hex(0x0d0917);
				iconPosition = saveIconPosition + ci::Vec2f(-37, 0);
			}

			void animateToMaxState(ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.9f)
			{
				bckgrndColor = ci::Color::hex(0x242135);
				iconPosition = saveIconPosition;
			}

			void animateToUpState(ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.9f)
			{
				animatePosition = _localPosition;
				timeline().apply( &animatePosition, _localPosition + ci::Vec2f(0, -400), time, eFunc)
					     .updateFn(bind( &Logo::posAnimationUpdate, this));
			}

			void animateToDownState(EaseFn eFunc = EaseOutCubic(), float time = 0.9f)
			{
				animatePosition = _localPosition;
				timeline().apply( &animatePosition, _localPosition + ci::Vec2f(0, 400), time, eFunc)
					     .updateFn(bind( &Logo::posAnimationUpdate, this));
			}		

			void posAnimationUpdate()
			{
				setPosition(animatePosition.value());
			}

		private:
			ci::Vec2f iconPosition, saveIconPosition;
			ci::gl::Texture icon;
			ci::Color bckgrndColor;
			int blockWidth, blockHeight;
			ci::Anim<ci::Vec2f> animatePosition;
		};

		typedef std::shared_ptr<Logo> LogoRef;
	}
}