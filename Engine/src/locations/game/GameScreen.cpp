#include "GameScreen.h"

using namespace kubik;

GameScreen::GameScreen(int gameID)
{	
	this->gameID = gameID;

	switch (gameID)
	{
		case gameId::FUNCES:
			currentGame = shared_ptr<IGame>(new Funces());
		break;

		case gameId::PHOTOBOOTH:
			currentGame = shared_ptr<IGame>(new Photobooth());
		break;

		default:
		break;
	}
	
	currentGame->closeGameSignal.connect(bind(&GameScreen::closeGameHandler, this));	
}

GameScreen::~GameScreen()
{
	console()<<"~~~~~~~~~~~~~~game screen destroy~~~~~~~~~~~~~~~~~~~~~~"<<endl;
}

void GameScreen::closeGameHandler()
{
	closeLocationSignal();
}

void GameScreen::draw()
{
	currentGame->draw();
}

void GameScreen::init(GameSettings* config)
{
	switch (gameID)
	{
		case gameId::FUNCES:
			currentGame->init(config->getFuncesSettings());
		break;

		case gameId::PHOTOBOOTH:
			currentGame->init(config->getPhotoboothSettings());
		break;

		default:
		break;
	}	
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