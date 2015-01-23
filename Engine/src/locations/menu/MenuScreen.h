#pragma once

#include "MenuButton.h"
#include "MenuSettings.h"
#include "IScreen.h"

#include "gui/TextButton.h"
#include "gui/EventGUI.h"
#include "gui/CompositeDispatcher.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

namespace kubik
{
	class MenuScreen:public IScreen, public CompositeDispatcher
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

		void gameMouseUpListener(EventGUIRef& button);
		void settingsMouseUpListener(EventGUIRef& button);
		void videoMouseUpListener(EventGUIRef& button);

		void createMenuBtns(vector<GamesInfo> gameIDs);
		void clearButtonVector();
		void update();

		Rectf getMenuBtuttonArea(int i);

		MenuSettingsRef settings;	
		TextButtonRef settingsButton, videoButton;
		gl::Texture bckgnd;
		Font font;

		list<SimpleButtonRef> btn;
	};

	typedef shared_ptr<MenuScreen> MenuScreenRef;	
}