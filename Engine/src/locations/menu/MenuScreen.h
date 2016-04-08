#pragma once

#include "MenuSettings.h"
#include "IScreen.h"
#include "gui/EventGUI.h"
#include "gui/Sprite.h"
#include "gui/SimpleSpriteButton.h"
#include "GameButton.h"
#include "ScreenStorage.h"
#include "GameChoosedEvent.h"
#include "TimerTools.h"

namespace kubik
{
	namespace menu
	{
		typedef std::shared_ptr<class MenuScreen> MenuScreenRef;

		class MenuScreen:public IScreen, public Sprite
		{
		public:
			MenuScreen(config::ISettingsRef config);
			~MenuScreen();

			enum locationState
			{
				SHOW_ANIM,
				INIT_ANIM,
				DRAW
			}state;

			void init(config::ISettingsRef  config) override;
			void reset() override{};

			ci::signals::signal<void(GameId)>  startGameSignal;
			SignalVoid startVideoSignal;

			void start();			
			void stop() override;
			void draw();	
			void resetMenuBtnGames();

		private:
			static const float goToScreenSaverTime;
			ci::signals::connection appUpdateSignal;
			ci::signals::connection mouseListener;

			void startGameHandler(EventGUIRef& button);
			
			void createMenuBtns(const std::vector<config::GameData>& gameIDs);
			void clearGamesButtonVector();
			void update();

			void showAnimationComplete();
			void gotoScreeenSaverTimeOut();

			//ci::Rectf getMenuBtuttonArea(int i);
			void drawShowAnim();
			void drawInitAnim();
			config::MenuSettingsRef settings;
			SimpleSpriteButtonRef settingsButton, videoButton;
			ci::gl::Texture bckgnd;
			
			std::list<SimpleSpriteButtonRef> gamesBtns;
			ci::Anim<float> animX1, animX, alpha;
		};
	}
}