#pragma once
#include "MenuSettings.h"
#include "IScreen.h"
#include "gui/EventGUI.h"
#include "gui/Sprite.h"
#include "gui/SimpleSpriteButton.h"
#include "GameButton.h"
#include "ScreenStorage.h"
#include "GameChoosedEvent.h"

namespace kubik
{
	namespace menu
	{
		typedef std::shared_ptr<class MenuScreen> MenuScreenRef;

		class MenuScreen:public IScreen, public Sprite
		{
		public:
			MenuScreen(ISettingsRef config);
			~MenuScreen();

			enum locationState
			{
				SHOW_ANIM,
				INIT_ANIM,
				DRAW
			}state;

			void init(ISettingsRef  config) override;
			void reset() override{};

			signal<void(GameId)>  startGameSignal;
			SignalVoid startVideoSignal;

			void start();			
			void stop() override;
			void draw();	
			void resetMenuBtnGames();

		private:		
			connection appUpdateSignal;
			connection mouseListener;	

			void startGameHandler(EventGUIRef& button);
			void videoMouseUpListener(EventGUIRef& button);

			void createMenuBtns(const std::vector<GameData>& gameIDs);
			void clearGamesButtonVector();
			void update();

			void showAnimationComplete();

			ci::Rectf getMenuBtuttonArea(int i);
			void drawShowAnim();
			void drawInitAnim();
			MenuSettingsRef settings;
			SimpleSpriteButtonRef settingsButton, videoButton;
			ci::gl::Texture bckgnd, screenshot;
			ci::Font font;

			std::list<SimpleSpriteButtonRef> gamesBtns;
			ci::Anim<float> animX1, animX, alpha;
		};
	}
}