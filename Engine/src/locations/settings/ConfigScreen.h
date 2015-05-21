#pragma once
#include "ApplicationModel.h"
#include "IScreen.h"
#include "ConfigSettings.h"
#include "MenuSettings.h"
#include "GameSettings.h"
#include "ScreenSaverSettings.h"
#include "Types.h"
#include "CloseConfigEvent.h"
#include "main/MainConfig.h"
#include "photobooth/PhotoboothConfig.h"
#include "InstakubConfig.h"
#include "gui/Sprite.h"
#include "GameSettingsSprite.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ConfigScreen> ConfigScreenRef;

		class ConfigScreen:public IScreen, public Sprite
		{
		public:
			ConfigScreen(ISettingsRef config);
			~ConfigScreen();

			void start();
			void stop();
			void draw();	
			void init();
			void init(ISettingsRef settings);

			void reset() override{};
			void startUpParams();
			void savePhtbtn();

			SignalVoid closeSettingsSignal;
			signal<void(std::vector<Changes>, bool)> appSettingsChangedSignal;
		
			void setScreenSaverSettings(ScreenSaverSettingsRef screenSaverSettings);
			void setMenuSettings(MenuSettingsRef menuSettings);
			void setGameSettings(GameSettingsRef gameSettings);

		private:			
			enum locationStates
			{
				SHOW_ANIM,
				DRAW
			}
			state;			

			ConfigSettingsRef		configSettings;
			ScreenSaverSettingsRef  screenSaverSettings;
			MenuSettingsRef			menuSettings;
			GameSettingsRef			gameSettings;		
			GameSettingsSpriteRef	gameSettingsScreen;
					
			MainConfigRef mainConfig; 
			PhotoboothConfigRef photoboothConfig;
			InstakubConfigRef instakubConfig;

			std::vector<Changes> changes;
			std::list<ISettingsRef> settingsList;

			ci::Anim<float> animX1, animX, alpha;

			void update();
			void closeLocationHandler();
			void gamesBlockHandler(EventGUIRef& event);
		
			void savePhotoboothParams();
			void createPhotoboothParams();
			void createMenuParams();	
			void createFuncesParams();	
			
			void checkMenuParamsForChanges();
			void checkGamesParamsForChanges();
			void checkScreenSaverParamsForChanges();
			void createGamesParams();		
			void createScreensaverParams();	
		
			void showingMainConfAnimationComplete();
			void initShowAnimation();
			void showAnimationComplete();
		};		
	}
}