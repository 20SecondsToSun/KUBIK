#pragma once
#include "gui/Sprite.h"
#include "TextItem.h"
#include "ConfigSettings.h"
#include "TextTools.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class StatBlock> StatBlockRef;

		class StatBlock :public Sprite
		{
		public:

			StatBlock(ConfigSettingsRef configSettings, const ci::Vec2i& position);

			virtual void drawLayout();

			void setAlpha(float alpha);
			void setPlayedTimes(int num);
			void setPrintedPhotos(int num);
			void setSharedAndEmail(int num);
			void nullValues();

		private:
			ci::Font numsFont;

			TextItem title1, title2, title3;
			ci::ColorA linesColor, numsColor;

			int playedTimes;
			int printedPhotos;
			int sharedAndEmail;

			Line<ci::Vec2f, ci::Vec2f> line1, line2, line3;
		};
	}
}