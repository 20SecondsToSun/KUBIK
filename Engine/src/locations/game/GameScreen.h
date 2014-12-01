#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "Drawable.h"


using namespace std;
using namespace ci;
using namespace ci::gl;
using namespace ci::app;

class GameScreen: public Drawable
{
public:
	//typedef boost::signals2::signal<void(MenuButton&)> ButtonSignal;	
	
	//ButtonSignal mouseUpSignal;

	GameScreen(int gameId, Graphics::OneBlockTexDictionary* dic);	

	void draw();

private:

	
};