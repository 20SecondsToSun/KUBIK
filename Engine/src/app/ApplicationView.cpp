#include "ApplicationView.h"

ApplicationView::ApplicationView()
{
	preloading = true;
	preloader = new Preloader(Vec2f(600,300));
}

void ApplicationView::setPreloading(bool value)
{
	preloading = value;
}

void ApplicationView::init(MenuScreen* menu, SettingsScreen* settings)
{
	this->menu = menu;
	this->settings = settings;
	this->game = game;		

	appUpdateSignal = App::get()->getSignalUpdate().connect( bind( &ApplicationView::update, this ));	

	preloading = false;
}

void ApplicationView::startMenu()
{
	console()<<"start menu ::  "<<endl;
	location = menu;
}

void ApplicationView::startSettings()
{
	console()<<"start settings ::  "<<endl;
	location = settings;
}

void ApplicationView::startGame(GameScreen * game)
{
	console()<<"start game ::  "<<endl;
	location = game;
}

void ApplicationView::startSettingsHandler()
{

}

void ApplicationView::setCurrentState(int value)
{
	
}

void ApplicationView::update()
{

}

void ApplicationView::draw()
{
	if(preloading)
		preloader->draw();
	else 
		location->draw();
	
}

