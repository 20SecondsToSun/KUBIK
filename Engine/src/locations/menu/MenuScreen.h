#pragma once
#include "cinder/app/AppNative.h"
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

		MenuScreen(shared_ptr<ISettings> config);
		~MenuScreen();

		void init(shared_ptr<ISettings>  config) override;
		void reset(shared_ptr<ISettings> config) override{};
		signal<void(game::id)>  startGameSignal;
		signal<void(void)> startSettingsSignal;
		signal<void(void)> startVideoSignal;

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
		void mouseUp( MouseEvent &event);
		void update();

		shared_ptr<MenuSettings> settings;
		shared_ptr<ButtonText> settingsButton, videoButton;
		gl::Texture bckgnd;
		Font font;
	};
}