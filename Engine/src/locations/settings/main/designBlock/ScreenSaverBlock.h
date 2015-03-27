#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "SettingsFactory.h"
#include "LoadButton.h"
#include "ScreenSaverSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ScreenSaverBlock> ScreenSaverBlockRef;

		class ScreenSaverBlock : public Sprite
		{
		public:
			static const int SCREEN_SAVER_STATE = 0;
			static const int SCREEN_SAVER_OPEN_FOLDER = 1;

			ScreenSaverBlock(ConfigSettingsRef configSettings, ScreenSaverSettingsRef ssSettings, const ci::Vec2i& position);

			bool getScreenSaverValue() const;
			virtual void activateListeners() override;
			virtual void unActivateListeners() override;
			virtual void drawLayout();

			void checkerClicked(EventGUIRef event);
			void openDirectory(EventGUIRef event);

			void drawDecorationLine();
			void setAlpha(float alpha);

		private:
			ConfigSettingsRef configSettings;
			LoadButtonRef loadButton;
			ScreenSaverCheckerRef checker;

			float lineWidth;
			float lineLength;
			ci::Color lineColor;
		};
	}
}