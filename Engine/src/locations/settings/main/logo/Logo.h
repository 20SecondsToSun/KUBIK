#pragma once
#include "gui/Sprite.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class Logo> LogoRef;

		class Logo : public Sprite
		{
		public:
			Logo(ConfigSettingsRef configSettings, const ci::Vec2i& position);

			virtual void drawLayout();

			void setIcon(const ci::gl::Texture& tex);
			float getWidth() const;

			void animateToMiniState(const ci::gl::Texture& icon, const ci::EaseFn& eFunc = ci::EaseOutCubic(), float time = 0.9f);
			void animateToMaxState(const ci::EaseFn& eFunc = ci::EaseOutCubic(), float time = 0.9f);
			void animateToUpState(const ci::EaseFn& eFunc = ci::EaseOutCubic(), float time = 0.9f);
			void animateToDownState(const ci::EaseFn& eFunc = ci::EaseOutCubic(), float time = 0.9f);
			void posAnimationUpdate();

		private:
			ci::Vec2f iconPosition, saveIconPosition;
			ci::gl::Texture icon, gameIcon;
			ci::Color bckgrndColor;
			ci::Anim<ci::Vec2f> animatePosition;
			bool isMiniState;
			float iconScale;
			int blockWidth, blockHeight;
		};
	}
}