#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IDrawable.h"
#include "Funces.h"
#include "Photobooth.h"
#include "Types.h"
#include "ISettings.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class GameScreen: public IGame
{
public:
	
	GameScreen(int gameId, ISettings* config);	
	~GameScreen();	

	Types::OneBlockTexDictionary getTextures();

	void draw();
	void init();
	void reset();

	void closeGameHandler();
	void addMouseUpListener();
	void removeMouseUpListener();

private:
	shared_ptr<IGame> currentGame;
	int gameID;	
};