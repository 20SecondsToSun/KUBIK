#pragma once
#include "MenuSettings.h"
#include "IScreen.h"
#include "gui/EventGUI.h"
#include "gui/Sprite.h"
#include "gui/SimpleSpriteButton.h"

namespace kubik
{
	namespace menu
	{
		class MenuScreen:public IScreen, public Sprite
		{
		public:
			MenuScreen(ISettingsRef config);
			~MenuScreen();

			void init(ISettingsRef  config) override;
			void reset() override{};

			signal<void(GameId)>  startGameSignal;
			SignalVoid startSettingsSignal;
			SignalVoid startVideoSignal;

			void start();
			void stop() override;
			void draw();	
			void resetMenuBtnGames();

		private:		
			connection appUpdateSignal;
			connection mouseListener;	

			void startGameHandler(EventGUIRef& button);
			void startSettingsHandler(EventGUIRef& button);
			void videoMouseUpListener(EventGUIRef& button);

			void createMenuBtns(const std::vector<GamesInfo>& gameIDs);
			void clearGamesButtonVector();
			void createControlsButtons();
			void update();

			ci::Rectf getMenuBtuttonArea(int i);

			MenuSettingsRef settings;	
			SimpleSpriteButtonRef settingsButton, videoButton;
			ci::gl::Texture bckgnd;
			ci::Font font;

			std::list<SimpleSpriteButtonRef> gamesBtns;
		};

		typedef shared_ptr<MenuScreen> MenuScreenRef;
	}
}