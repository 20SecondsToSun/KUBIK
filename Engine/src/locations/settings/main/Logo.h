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
				saveIconPosition =
				iconPosition	 =  Vec2f(0.5 * (blockWidth - icon.getWidth()),
										  0.5 * (blockHeight - icon.getHeight()));
			}

			float getWidth()
			{
				return blockWidth;
			}

			void animateToMiniState(EaseFn eFunc = EaseOutCubic(), float time = 0.9f)
			{
				bckgrndColor = Color::hex(0x0d0917);
				iconPosition = saveIconPosition + Vec2f(-37, 0);
			}

			void animateToMaxState(EaseFn eFunc = EaseOutCubic(), float time = 0.9f)
			{
				bckgrndColor = Color::hex(0x242135);
				iconPosition = saveIconPosition;
			}

			void animateToUpState(EaseFn eFunc = EaseOutCubic(), float time = 0.9f)
			{
				animatePosition = _localPosition;
				timeline().apply( &animatePosition, _localPosition + Vec2f(0, -400), time, eFunc)
					     .updateFn(bind( &Logo::posAnimationUpdate, this));
			}

			void animateToDownState(EaseFn eFunc = EaseOutCubic(), float time = 0.9f)
			{
				animatePosition = _localPosition;
				timeline().apply( &animatePosition, _localPosition + Vec2f(0, 400), time, eFunc)
					     .updateFn(bind( &Logo::posAnimationUpdate, this));
			}		

			void posAnimationUpdate()
			{
				setPosition(animatePosition.value());
			}

		private:
			Vec2f iconPosition, saveIconPosition;
			Texture icon;
			Color bckgrndColor;
			int blockWidth, blockHeight;

			Anim<Vec2f> animatePosition;
		};

		typedef std::shared_ptr<Logo> LogoRef;
	}
}