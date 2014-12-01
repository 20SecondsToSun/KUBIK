#include "Application.h"

void Application::init(ApplicationModel *model, Graphics::AllTexDictionary texdic)
{
	this->model = model;
	this->texdic = texdic;

	// create menu
	menu = new MenuScreen(model->getGameIDsTurnOn(), texdic[Graphics::gamesTexturesID::MENU]);
	menu->startGameSignal.connect(bind(&Application::startGameHandler, this, std::placeholders::_1));	

	// create settings

	appUpdateSignal = App::get()->getSignalUpdate().connect( bind( &Application::update, this ));
	setCurrentState(MENU);
}

void Application::startGameHandler(int gameId)
{
	console()<<"game id::  "<<gameId<<endl;
	menu->removeMouseListener();

	if(model->isGameLoaded(gameId))
	{
		game = new GameScreen(gameId, texdic[(Graphics::gamesTexturesID)gameId]);
		setCurrentState(GAME);
	}
	else
	{
		//loadGameSignal(gameId);
	}
}

void Application::startSettingsHandler()
{

}

void Application::setCurrentState(int value)
{
	currentState = value;

	switch (currentState)
	{
		case MENU:
			location = menu;
		break;
		case GAME:
			location = game;
		break;
	}
}

void Application::update()
{

}

void Application::setNextState(int value)
{
	nextState = value;
}

void Application::draw()
{
	location->draw();
}