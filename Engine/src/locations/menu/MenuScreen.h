#pragma once
#include "cinder/app/AppNative.h"
#include "MenuButton.h"
#include "Button.h"
#include "IDrawable.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

class MenuScreen:public IDrawable
{
public:
	MenuScreen(vector<int> gameIDs);

	signal<void(int)> startGameSignal;
	signal<void(void)> startSettingsSignal;
	signal<void(void)> startVideoSignal;
	
	void draw();	
	void init();

	void removeMouseUpListener();
	void addMouseUpListener();	

private:
	vector<MenuButton *> menuBtns;

	connection appUpdateSignal;
	connection mouseListener;	

	void mouseUpListener(MenuButton& button);
	void settingsMouseUpListener(ButtonText& button);
	void videoMouseUpListener(ButtonText& button);

	void createMenuBtns(vector<int> gameIDs);
	void clearButtonVector();
	void mouseUp( MouseEvent &event);
	void update();
	void setTextures();

	ButtonText *settingsButton, *videoButton;

	ci::Font font;
};