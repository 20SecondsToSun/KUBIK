#pragma once
#include "gui/SimpleSpriteButton.h"
#include "TextTools.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class StartNewActivity> StartNewActivityRef;

		class StartNewActivity: public SimpleSpriteButton
		{
		public:			
			StartNewActivity(ConfigSettingsRef configSettings, const ci::Vec2i& position);

			virtual void drawLayout() override;
			virtual void setAlpha(float alpha) override;

		private:			
			ci::gl::Texture icon;
			TextItem textItem;
			ci::ColorA color;
		};		
	}
}