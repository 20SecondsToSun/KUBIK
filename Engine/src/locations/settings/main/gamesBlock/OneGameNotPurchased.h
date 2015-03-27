#pragma once
#include "gui/SimpleSpriteButton.h"
#include "settings/ConfigSettings.h"
#include "TextTools.h"
#include "settings/GameShowUrlEvent.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class OneGameNotPurchased> OneGameNotPurchasedRef;

		class OneGameNotPurchased: public SimpleSpriteButton
		{
		public:
			OneGameNotPurchased(ConfigSettingsRef config, const GamesInfo& info);
			virtual void drawLayout() override;
			void setAlpha(float alpha);

		private:
			std::string nameText;
			ci::Font nameFont;
			ci::ColorA nameColor, btnColor;
			ci::gl::Texture miniIcon, gameInWebIcon, textTexture;			
		};
	}
}