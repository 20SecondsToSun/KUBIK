#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "main/DesignBlock/ReturnButton.h"
#include "main/DesignBlock/ScreenSaverBlock.h"
#include "ConfigSettings.h"
#include "DesignChooser.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class DesignsLayout> DesignsLayoutRef;

		class DesignsLayout: public Sprite
		{
		public:	
			static const int SCREEN_SAVER_STATE = 0;
			static const int SCREEN_SAVER_OPEN_FOLDER = 1;
			static const int CHANGED_DESIGN = 2;
			static const int OPEN_USER_DESIGN_FOLDER = 3;	
			static const int HIDE = 4;	

			DesignsLayout(ConfigSettingsRef configSettings, const ci::Vec2i& position);
			
			virtual void activateListeners() override;
			virtual void unActivateListeners() override;

			void checkerChanged();
			void openScreenSaverFolder();
			void returnHandler(EventGUIRef& event);
			void designChanged();
			void openUserDesignFolder();
			bool getScreenSaverValue() const;
			int getDesignID() const;

		private:			
			ConfigSettingsRef	configSettings;
			ScreenSaverBlockRef screenSaver;
			ReturnButtonRef		returnButton;
			DesignChooserRef	designChooser;
		};		
	}
}