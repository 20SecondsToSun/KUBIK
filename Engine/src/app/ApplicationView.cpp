#include "ApplicationView.h"


void ApplicationView::init(ScreenSaver* screenSaver, MenuScreen* menu, SettingsScreen* settings)
{
	this->screenSaver = screenSaver;
	this->menu = menu;
	this->settings = settings;
	this->game = game;	
}

void ApplicationView::startLocation(IDrawable* screen)
{	
	location = screen;
}

void ApplicationView::draw()
{
	location->draw();
}