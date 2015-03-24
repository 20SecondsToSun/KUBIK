#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "Utils.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class Title> TitleRef;

		class Title :public Sprite
		{
		public:
			Title(ConfigSettingsRef configSettings, const ci::Vec2i& position);

			virtual void drawLayout();

			void setAlpha(float alpha);
			void setActivityName(const std::string& name);

		private:
			std::string name;
			ci::ColorA  color;
			ci::Font font;
		};
	}
}