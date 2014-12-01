#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "MenuScreen.h"
#include "GameScreen.h"
#include "Graphics.h"
#include "Drawable.h"


using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;

class Application
{
public:
	enum states
	{
		MENU,
		SETTINGS,
		GAME
	};	

	void init(ApplicationModel *model, Graphics::AllTexDictionary texdic );
	void draw();

	void mouseUpListener(MenuButton& button);

private:
	connection appUpdateSignal;
	int currentState, nextState;
	ApplicationModel *model;
	Graphics::AllTexDictionary texdic;

	void setCurrentState(int value);
	void setNextState(int value);

	void update();
	void (Application::* drawApp)();	

	void clearButtonVector();

	MenuScreen *menu;
	GameScreen *game;

	void startGameHandler(int gameId);
	void startSettingsHandler();

	Drawable *location;
};