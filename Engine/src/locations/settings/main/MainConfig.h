#pragma once

#include "gui/Sprite.h"
#include "main/title/Title.h"
#include "main/startNewActivity/StartNewActivity.h"
#include "main/statBlock/StatBlock.h"
#include "main/designBlock/DesignBlock.h"
#include "main/gamesBlock/GamesBlock.h"
#include "main/printerBlock/PrinterBlock.h"
#include "main/logo/Logo.h"
#include "main/closeBlock/CloseBlock.h"
#include "main/popup/NewActivityPopup.h"
#include "GameCheckerEvent.h"
#include "GameSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class MainConfig> MainConfigRef;

		class MainConfig: public Sprite
		{
		public:
			static const int SHOW_ANIM_COMPLETE = 0; 

			MainConfig(ConfigSettingsRef configSettings, GameSettingsRef gameSettings);

			virtual void activateListeners();
			virtual void unActivateListeners();

			void killAll();
			void closeHandler(EventGUIRef& event);

			////////////////////////////////////////////////////////////////////////////
			//
			//				PRINTER CONTROLS
			//
			////////////////////////////////////////////////////////////////////////////

			void openPrinterControls();
			void printerControlsOpened();
			void printerHideHandler();
			void printerStatResetHandler(); 
			void hidePrinterControls();
			void printerControlsHided();

			////////////////////////////////////////////////////////////////////////////
			//
			//				POPUP
			//
			////////////////////////////////////////////////////////////////////////////			

			void openPopupNewActivity(EventGUIRef& event);			
			void popupOpened();
			void popupHideHandler();
			void popupStartNewCompainHandler();
			void startNewCampain();
			void closingPopup();
			void popupClosed();

			////////////////////////////////////////////////////////////////////////////
			//
			//				DESIGN BLOCK
			//
			////////////////////////////////////////////////////////////////////////////		

			void openingDesignLayout();
			void openedDesignLayout();
			void screenSaverStateChanged();
			void screenSaverOpenFolder();
			void changedKubikDesign();	
			void openUserDesignFolder();
			void hidingDesignLayout();			
			void hideDesignLayot();
			void closeDesignBlock();

			////////////////////////////////////////////////////////////////////////////
			//
			//			GAMES BLOCK	
			//
			////////////////////////////////////////////////////////////////////////////

			virtual void draw();

			void gamesBlockEventsHandler(EventGUIRef& event);
			void setStartupData();

			void startAnimation(ci::EaseFn eFunc = ci::EaseOutCubic(), float time = 0.9f);			
			void alphAnimationUpdate();

			void posAnimationUpdate();
			void startAnimationFinish();

			void hideAnimate(GameId id, ci::EaseFn eFunc, float time);
			void hideAnimationFinish();

			void showAnimate(ci::EaseFn eFunc, float time);
			void showAnimationFinish();

		private:
			ConfigSettingsRef	configSettings;
			GameSettingsRef		gameSettings;
			CloseBlockRef		closeBlock;
			TitleRef			title;
			StartNewActivityRef startNewActivity;
			StatBlockRef		statBlock;
			DesignBlockRef		designBlock;
			GamesBlockRef		gamesBlock;
			PrinterBlockRef		printerBlock;
			LogoRef				logo;
			NewActivityPopupRef	popup, newActPopup;
			
			ci::Anim<ci::Vec2f> animatePosition;
			ci::Anim<float> alpha;
		};		
	}
}