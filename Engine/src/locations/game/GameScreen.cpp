#include "GameScreen.h"

GameScreen::GameScreen(int gameID)
{	
	this->gameID = gameID;

	switch (gameID)
	{
		case 1:
			currentGame = shared_ptr<IGame>(new Funces());		
		break;
		case 2:
			currentGame = shared_ptr<IGame>(new Photobooth());
		break;

		case 3:
			//currentGame = new Kotopoza();
		break;

		default:
		break;
	}
	currentGame->closeGameSignal.connect(bind(&GameScreen::closeGameHandler, this));	
}

GameScreen::~GameScreen()
{
	console()<<"game screen destrpy------------------->"<<endl;
}

void GameScreen::closeGameHandler()
{
	closeGameSignal();
}

void GameScreen::draw()
{	
	currentGame->draw();
}

void GameScreen::init()
{
	currentGame->init();
}

void GameScreen::create()
{
	currentGame->create();
}

OneBlockTexDictionary GameScreen::getTextures()
{
	return currentGame->getTextures();
}

void GameScreen::removeMouseUpListener()
{
	currentGame->removeMouseUpListener();
};

void GameScreen::addMouseUpListener()
{
	currentGame->addMouseUpListener();
};