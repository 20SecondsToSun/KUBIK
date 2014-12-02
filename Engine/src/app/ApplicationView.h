#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "MenuScreen.h"
#include "GameScreen.h"
#include "SettingsScreen.h"
#include "Graphics.h"
#include "IDrawable.h"
#include "Preloader.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

class ApplicationView
{
public:
	enum states
	{
		MENU,
		SETTINGS,
		GAME
	};	
	ApplicationView();

	void init(MenuScreen* menu, SettingsScreen* settings);
	void draw();
	void startGame(GameScreen * game);
	void startMenu();
	void startSettings();
	void setPreloading(bool value);

private:
	connection appUpdateSignal;	
	ApplicationModel *model;

	void setCurrentState(int value);
	void setNextState(int value);

	void update();
	void (ApplicationView::* drawApp)();	

	void clearButtonVector();

	
	void startSettingsHandler();

	bool preloading;

	IDrawable *location;
	MenuScreen* menu;
	SettingsScreen* settings;
	GameScreen* game;

	Preloader* preloader;
};