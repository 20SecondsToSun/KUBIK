#include "GameScreen.h"

using namespace kubik;

GameScreen::GameScreen(int gameID)
{	
	this->gameID = gameID;
	create();	
}

GameScreen::~GameScreen()
{
	console()<<"~~~~~~~~~~~~~~game screen destroy~~~~~~~~~~~~~~~~~~~~~~"<<endl;
	currentGame->closeGameSignal.disconnect_all_slots();
}

void GameScreen::reload()
{	
	create();
}

void GameScreen::closeGameHandler()
{
	console()<<"CLOSE GAME:::"<<endl;
	closeLocationSignal();
}

void GameScreen::draw()
{
	currentGame->draw();
}

void GameScreen::init(GameSettings* config)
{
	if(config->getCurrentGame() == gameID)
		currentGame->init(config->getGameSettingsById());	
}

void GameScreen::create()
{
	switch (gameID)
	{
		case gameId::FUNCES:
			currentGame = shared_ptr<IGame>(new Funces());
		break;

		case gameId::PHOTOBOOTH:
			currentGame = shared_ptr<IGame>(new Photobooth());
		break;	
	}
	connect_once(currentGame->closeGameSignal, bind(&GameScreen::closeGameHandler, this));	
}

void GameScreen::reset()
{
	currentGame->reset();
}

void GameScreen::removeMouseUpListener()
{
	currentGame->removeMouseUpListener();
};

void GameScreen::addMouseUpListener()
{
	currentGame->addMouseUpListener();
};