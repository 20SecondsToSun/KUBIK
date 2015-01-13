#pragma once

#include "MenuButton.h"
#include "ButtonText.h"
#include "MenuSettings.h"
#include "IScreen.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

namespace kubik
{
	class MenuScreen:public IScreen
	{
	public:
		MenuScreen(ISettingsRef config);
		~MenuScreen();

		void init(ISettingsRef  config) override;
		void reset() override{};

		signal<void(game::id)>  startGameSignal;
		SignalVoid startSettingsSignal;
		SignalVoid startVideoSignal;

		void start();
		void stop() override;

		void draw();
		void removeMouseUpListener();
		void addMouseUpListener();

		void resetMenuBtnGames();

	private:
		vector<shared_ptr<MenuButton>> menuBtns;

		connection appUpdateSignal;
		connection mouseListener;	

		void mouseUpListener(MenuButton& button);
		void settingsMouseUpListener(ButtonText& button);
		void videoMouseUpListener(ButtonText& button);

		void createMenuBtns(vector<GamesInfo> gameIDs);
		void clearButtonVector();
		void mouseUp(MouseEvent &event);
		void update();

		shared_ptr<MenuSettings> settings;
		shared_ptr<ButtonText> settingsButton, videoButton;
		gl::Texture bckgnd;
		Font font;
	};

	typedef shared_ptr<MenuScreen> MenuScreenRef;	
}