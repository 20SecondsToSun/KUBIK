#pragma once

#include "MenuButton.h"
#include "ButtonText.h"
#include "MenuSettings.h"
#include "IScreen.h"
#include "IDispatcher.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

namespace kubik
{
	class MenuScreen:public IScreen, public IDispatcher
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

		void gameMouseUpListener(EventRef& button);
		void settingsMouseUpListener(EventRef& button);
		void videoMouseUpListener(EventRef& button);

		void createMenuBtns(vector<GamesInfo> gameIDs);
		void clearButtonVector();
		void update();

		MenuSettingsRef settings;
		ButtonTextRef settingsButton, videoButton;
		gl::Texture bckgnd;
		Font font;
	};

	typedef shared_ptr<MenuScreen> MenuScreenRef;	
}