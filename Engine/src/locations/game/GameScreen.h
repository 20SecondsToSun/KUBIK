#pragma once
#include "cinder/app/AppNative.h"

#include "IDrawable.h"
#include "Funces.h"
#include "Photobooth.h"
#include "Types.h"
#include "ISettings.h"

using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class GameScreen: public IDrawable
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
};