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
				 blockHeight(170),
				 isMiniState(false)				 
			{				
				 setIcon(configSettings->getTexture("logoIcon"));
				 setPosition(position);
			}

			virtual void drawLayout()
			{				
				gl::color(bckgrndColor);
				//gl::color(Color(1,0,0));
				gl::drawSolidRect(ci::Rectf(ci::Vec2f::zero(), ci::Vec2f(blockWidth, blockHeight)));
				gl::color(ci::Color::white());
				gl::draw(icon, iconPosition);	

				if (isMiniState)
				{
					gl::translate(0.5f * gameIcon.getSize());
					gl::scale(iconScale, iconScale);
					gl::translate(-0.5f * iconScale * gameIcon.getSize());
					gl::translate(0.0f, -123.0f);
					gl::draw(gameIcon);	
				}
			}

			void setIcon(const ci::gl::Texture& tex)
			{
				icon = tex;
				saveIconPosition =
				iconPosition	 =  ci::Vec2f(0.5f * (blockWidth - icon.getWidth()),
											  0.5f * (blockHeight - icon.getHeight()));
			}

			float getWidth() const
			{
				return blockWidth;
			}

			void animateToMiniState(ci::gl::Texture icon, ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.9f)
			{
				bckgrndColor = ci::Color::hex(0x0d0917);
				iconPosition = saveIconPosition + ci::Vec2f(-37.0f, 0.0f);
				isMiniState = true;
				gameIcon = icon;

				iconScale = 100.0f / gameIcon.getWidth();
			}

			void animateToMaxState(ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.9f)
			{
				bckgrndColor = ci::Color::hex(0x242135);
				iconPosition = saveIconPosition;
				isMiniState = false;
			}

			void animateToUpState(ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.9f)
			{
				animatePosition = _localPosition;
				timeline().apply( &animatePosition, _localPosition + ci::Vec2f(0.0f, -400.0f), time, eFunc)
					     .updateFn(bind( &Logo::posAnimationUpdate, this));
			}

			void animateToDownState(EaseFn eFunc = EaseOutCubic(), float time = 0.9f)
			{
				animatePosition = _localPosition;
				timeline().apply( &animatePosition, _localPosition + ci::Vec2f(0.0f, 400.0f), time, eFunc)
					     .updateFn(bind( &Logo::posAnimationUpdate, this));
			}		

			void posAnimationUpdate()
			{
				setPosition(animatePosition.value());
			}

		private:
			ci::Vec2f iconPosition, saveIconPosition;
			ci::gl::Texture icon, gameIcon;
			ci::Color bckgrndColor;
			int blockWidth, blockHeight;
			ci::Anim<ci::Vec2f> animatePosition;
			bool isMiniState;	

			float iconScale;
		};

		typedef std::shared_ptr<Logo> LogoRef;
	}
}