#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Timeline.h"
#include "cinder/gl/Texture.h"
#include "cinder/Timer.h"
#include "Location.h"

using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace gl;
using namespace std;

class PlayingGameScreen : public Location
{
public:

	void setup();
	void init( LocationEngine* game);
	void update();
	void draw();
	void cleanup(){};

	void handleEvents(){};
	void mouseEvents(int type){};
	void keyEvents(){};		

	static PlayingGameScreen* Instance()
	{
		return &PlayingGameScreenState;
	}

private:

	LocationEngine* _game;
	static PlayingGameScreen PlayingGameScreenState;

};