#include "ApplicationView.h"

ApplicationView::ApplicationView()
{	

}

void ApplicationView::init(ScreenSaver* screenSaver, MenuScreen* menu, SettingsScreen* settings)
{
	this->screenSaver = screenSaver;
	this->menu = menu;
	this->settings = settings;
	this->game = game;		

	appUpdateSignal = App::get()->getSignalUpdate().connect( bind( &ApplicationView::update, this ));
}

void ApplicationView::startLocation(IDrawable* screen)
{	
	location = screen;
}

void ApplicationView::update()
{

}

void ApplicationView::draw()
{
	location->draw();
}