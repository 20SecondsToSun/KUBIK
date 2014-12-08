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

		signal<void(int)> startGameSignal;
		signal<void(void)> startSettingsSignal;
		signal<void(void)> startVideoSignal;

		void draw();	
		void clean();
		void reload();
		void init(MenuSettings* config);

		void removeMouseUpListener();
		void addMouseUpListener();

	private:
		vector<MenuButton*> menuBtns;

		connection appUpdateSignal;
		connection mouseListener;	

		void mouseUpListener(MenuButton& button);
		void settingsMouseUpListener(ButtonText& button);
		void videoMouseUpListener(ButtonText& button);

		void createMenuBtns(vector<int> gameIDs);
		void clearButtonVector();
		void mouseUp( MouseEvent &event);
		void update();

		MenuSettings *settings;
		ButtonText *settingsButton, *videoButton;
		gl::Texture bckgnd;
		ci::Font font;
	};
}