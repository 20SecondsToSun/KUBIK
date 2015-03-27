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
			void calculateDigitTextures();

		private:
			ci::Font numsFont1, numsFont2, numsFont3;

			TextItem title1, title2, title3;
			ci::ColorA linesColor, numsColor;

			gl::Texture playedTimesTexture, printedPhotosTexture, sharedAndEmailTexture;
			ci::Vec2f playedTimesPosition, printedPhotosPosition, sharedAndEmailPosition;

			int playedTimes;
			int printedPhotos;
			int sharedAndEmail;

			Line<ci::Vec2f, ci::Vec2f> line1, line2, line3;
		};
	}
}