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
	MenuScreen();

	signal<void(int)> startGameSignal;
	signal<void(void)> startSettingsSignal;
	signal<void(void)> startVideoSignal;
	
	void draw();	

	virtual void init(){};
	void init(vector<int> gameIDs);

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