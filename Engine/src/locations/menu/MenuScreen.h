#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "Drawable.h"


using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

class MenuScreen:public Drawable
{
public:
	signal<void(int)> startGameSignal;
	
	MenuScreen(vector<int> gameIDs, Graphics::OneBlockTexDictionary* dic);
	void draw();	

	void removeMouseListener();	

private:
	vector<MenuButton *> menuBtns;

	connection appUpdateSignal;
	connection mouseListener;	

	void mouseUpListener(MenuButton& button);
	void createMenuBtns(vector<int> gameIDs);
	void clearButtonVector();
	void mouseUp( MouseEvent &event);
	void update();
};