#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IDrawable.h"


using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

class MenuScreen:public IDrawable
{
public:
	signal<void(int)> startGameSignal;
	
	MenuScreen(vector<int> gameIDs);
	void draw();	

	void removeMouseUpListener();
	void addMouseUpListener();

	void init();

	void trace()
	{
		console()<<designTexures["background"]->isLoading<<"  "<<designTexures["background"]->path<<"  "<<designTexures["background"]->tex<<endl;
	}

private:
	vector<MenuButton *> menuBtns;

	connection appUpdateSignal;
	connection mouseListener;	

	void mouseUpListener(MenuButton& button);
	void createMenuBtns(vector<int> gameIDs);
	void clearButtonVector();
	void mouseUp( MouseEvent &event);
	void update();
	void setTextures();
};