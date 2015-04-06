#pragma once
#include "gui/Sprite.h"
#include "InstakubSettings.h"
#include "instakub/states/InstagramView.h"

using namespace kubik::config;

namespace kubik
{
	namespace games
	{
		namespace instakub
		{
			typedef std::shared_ptr<class InstakubLocation> InstakubLocationRef;

			class InstakubLocation : public Sprite
			{
			public:
				static const int ENABLE_CONTROLS = 1;
				static const int DISABLE_CONTROLS = 2;
				static const int SHOW_CONTROLS = 3;
				static const int HIDE_CONTROLS = 4;

			protected:
				static ci::gl::Texture bg;
				ci::gl::Texture title, overMask;
				ci::Vec2f titlePosition, searchFieldPosition;

				static InstagramViewRef instagramView;

			public:
				InstakubLocation(InstakubSettingsRef settings);
				virtual void start() = 0;
				virtual void draw();
				virtual void stop() = 0;
				virtual void reset();
			
				void fillBg();
				void drawTitle();

			protected:
				InstakubSettingsRef settings;

			private:
				void enableControls();
				void disableControls();
				void showControls();
				void hideControls();
			};
		}
	}
}