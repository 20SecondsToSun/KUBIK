#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuScreen.h"
#include "GameScreen.h"
#include "SettingsScreen.h"
#include "IDrawable.h"
#include "Preloader.h"
#include "ScreenSaver.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

class ApplicationView
{
public:
	ApplicationView();

	void init(ScreenSaver* screenSaver, MenuScreen* menu, SettingsScreen* settings);
	void draw();
	void startLocation(IDrawable* screen);

private:	
	ApplicationModel *model;

	IDrawable *location;
	MenuScreen* menu;
	SettingsScreen* settings;
	GameScreen* game;
	ScreenSaver* screenSaver;
	Preloader* preloader;

	connection appUpdateSignal;	

	void update();
};