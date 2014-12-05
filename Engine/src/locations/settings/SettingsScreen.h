#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IDrawable.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class SettingsScreen:public IDrawable
{
public:
	signal<void(int)> startGameSignal;
	
	SettingsScreen();
	void draw();	
	void init();

	signal<void(void)> closeSettingsSignal;

	void addMouseUpListener();
	void removeMouseUpListener();

	connection mouseUpListener;

private:
	void update();
	void setTextures();
	void mouseUp(MouseEvent &event);

	Font font;
};