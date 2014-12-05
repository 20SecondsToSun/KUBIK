#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IGame.h"
#include "Button.h"
#include "ISettings.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class Funces:public IGame
{
public:	
	
	Funces(ISettings* config);
	~Funces();

	void draw();	
	void init();
	void reset();

	void addMouseUpListener();
	void removeMouseUpListener();

private:	

	Texture closeImg;
	Button *closeBtn;

	void setTextures();
	void mouseUp( MouseEvent &event);
	void mouseUpHandler(Button& button );

	connection mouseUpListener, closeBtnListener;	
};