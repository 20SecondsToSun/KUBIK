#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IScreen.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class SettingsScreen:public IScreen
{
public:

	SettingsScreen();

	signal<void(int)> startGameSignal;	

	void draw();	
	void init();

	signal<void(void)> closeSettingsSignal;

	void addMouseUpListener();
	void removeMouseUpListener();

	connection mouseUpListener;

protected:
	void setTextures();

private:
	void update();
	
	void mouseUp(MouseEvent &event);

	Font font;
};