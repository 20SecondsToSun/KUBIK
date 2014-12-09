#include "ApplicationView.h"

using namespace kubik;

void ApplicationView::init(ScreenSaver* screenSaver, MenuScreen* menu, TuneUpScreen* settings)
{
	this->screenSaver = screenSaver;
	this->menu = menu;
	this->settings = settings;
	this->game = game;	
}

void ApplicationView::startLocation(IScreen* screen)
{	
	location = screen;
}

void ApplicationView::draw()
{
	location->draw();
}