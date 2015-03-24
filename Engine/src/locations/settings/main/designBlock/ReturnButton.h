#pragma once

#include "TextTools.h"
#include "gui/SimpleSpriteButton.h"
#include "CloseDesignLayoutEvent.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ReturnButton> ReturnButtonRef;

		class ReturnButton : public SimpleSpriteButton
		{
		public:
			ReturnButton(ConfigSettingsRef configSettings, const ci::Vec2i& position);
			virtual void drawLayout();
			void setAlpha(float  alpha);

		private:
			ci::gl::Texture icon, textTexture;
			TextItem text;
			float startX;
		};
	}
}